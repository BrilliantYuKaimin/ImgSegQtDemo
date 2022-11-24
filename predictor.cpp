//
// Created on 2022/11/24.
//

#include <numeric>

#include "predictor.h"

std::shared_ptr<paddle_infer::Predictor> predictor;

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
    cv::equalizeHist(output_img, output_img);
    cv::cvtColor(output_img, output_img, cv::COLOR_GRAY2BGR);
    return output_img;
}