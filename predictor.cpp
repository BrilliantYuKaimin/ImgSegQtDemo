//
// Created on 2022/11/24.
//

#include <numeric>

#include "predictor.h"

std::shared_ptr<paddle_infer::Predictor> predictor;


/*
 把图像送入一个分割网络进行推理。

 参数:
   img (cv::Mat): 通道顺序为 RGB 的 3 通道图像

 返回:
   (cv::Mat) 单通道灰度图
 */
cv::Mat predict(cv::Mat img) {
    img.convertTo(img, CV_32F, 1.0 / 255, 0);
    img = (img - 0.5) / 0.5;
    int rows = img.rows;
    int cols = img.cols;
    int chs = img.channels();

    auto input_names = predictor->GetInputNames();
    auto input_tensor = predictor->GetInputHandle(input_names[0]);
    std::vector<int> input_shape = {1, chs, rows, cols};
    input_tensor->Reshape(input_shape);
    int input_num = std::accumulate(input_shape.begin(), input_shape.end(), 1, std::multiplies<int>());
    std::vector<float> input_data(input_num, 1.0);
    for (int i = 0; i <chs ; ++i) {
        cv::extractChannel(img, cv::Mat(rows, cols, CV_32FC1, input_data.data() + i * rows * cols), i);
    }
    input_tensor->CopyFromCpu(input_data.data());

    predictor->Run();

    auto output_names = predictor->GetOutputNames();
    auto output_tensor = predictor->GetOutputHandle(output_names[0]);
    std::vector<int> output_shape = output_tensor->shape();
    std::vector<int64> output_data;
    int output_num = std::accumulate(output_shape.begin(), output_shape.end(), 1, std::multiplies<int>());
    output_data.resize(output_num);
    output_tensor->CopyToCpu(output_data.data());

    std::vector<uint8_t> output_data_u8(output_num);
    for (int i = 0; i < output_num; ++i) {
        output_data_u8[i] = static_cast<uint8_t>(output_data[i]);
    }
    cv::Mat output_img(output_shape[1], output_shape[2], CV_8UC1, output_data_u8.data());
    return output_img;
}

/*
 把灰度的预测结果转成伪彩图像。

 参数:
   gray_img (cv::Mat): 一个单通道灰度图

 返回:
   (cv::Mat) 3 通道伪彩图像
 */
cv::Mat convert_to_pseudo_color(cv::Mat gray_img) {
    cv::Mat &output_img = gray_img;
    cv::cvtColor(output_img, gray_img, cv::COLOR_GRAY2BGR);
    for (int i = 0; i < output_img.rows; ++i) {
        auto p = output_img.ptr<cv::Vec3b>(i);
        for (int j = 0; j < output_img.rows; ++j) {
            {
                auto k = p[j][0];
                p[j][0] = color_map[3 * k + 0];
                p[j][1] = color_map[3 * k + 1];
                p[j][2] = color_map[3 * k + 2];
            }
        }
    }
    return output_img;
}

std::vector<uchar> color_map((256 + 1) * 3, 0);

void init_color_map_list(int num_classes) {
    for (int i = 0; i < num_classes + 1; ++i) {
        int j = 0;
        int lab = i;
        while (lab) {
            color_map[i * 3] |= (((lab >> 0) & 1) << (7 - j));
            color_map[i * 3 + 1] |= (((lab >> 1) & 1) << (7 - j));
            color_map[i * 3 + 2] |= (((lab >> 2) & 1) << (7 - j));
            j += 1;
            lab >>= 3;
        }
    }
    color_map.erase(color_map.begin(), color_map.begin() + 3);
}
