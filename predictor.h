//
// Created on 2022/11/24.
//

#ifndef IMGSEGQTDEMO_PREDICTOR_H
#define IMGSEGQTDEMO_PREDICTOR_H

#include <opencv2/opencv.hpp>
#include "paddle/include/paddle_inference_api.h"

extern std::shared_ptr<paddle_infer::Predictor> predictor;
cv::Mat predict(cv::Mat img);

#endif //IMGSEGQTDEMO_PREDICTOR_H
