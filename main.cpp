#include <iostream>

#include "paddle/include/paddle_inference_api.h"

int main() {
    paddle_infer::Config config;
    config.SetModel("../model.pdmodel", "../model.pdiparams");
    auto predictor = paddle_infer::CreatePredictor(config);
    return 0;
}
