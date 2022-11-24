#include <QApplication>
#include "paddle/include/paddle_inference_api.h"

#include "MainWindow.h"
#include "predictor.h"

int main(int argc, char *argv[]) {
    paddle_infer::Config config;
    config.SetModel("../model.pdmodel", "../model.pdiparams");
    predictor = paddle_infer::CreatePredictor(config);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
