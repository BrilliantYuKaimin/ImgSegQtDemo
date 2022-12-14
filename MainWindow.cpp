//
// Created on 2022/11/24.
//
#include <opencv2/opencv.hpp>
#include <numeric>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QMimeData>

#include "MainWindow.h"
#include "MainWindow_ui.h"
#include "predictor.h"

MainWindow::MainWindow(QMainWindow *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    paddle_infer::Config config;
    config.SetModel((QApplication::applicationDirPath() + "/../Resources/model.pdmodel").toStdString(),
                    (QApplication::applicationDirPath() + "/../Resources/model.pdiparams").toStdString());
    predictor = paddle_infer::CreatePredictor(config);
    init_color_map_list();
}

MainWindow::~MainWindow() noexcept {
    delete ui;
}

void MainWindow::forward_by_fileneme(const QString& filename) {
    auto src_img = cv::imread(filename.toStdString());
    cv::cvtColor(src_img, src_img, cv::COLOR_BGR2RGB);

    this->resize(src_img.cols, src_img.rows + 50);
    ui->label_img->resize(src_img.cols, src_img.rows);
    ui->label_1->resize(src_img.cols - 90, 25);
    ui->label_2->resize(src_img.cols - 90, 25);

    ui->label_1->setText(filename);
    ui->label_2->setText(QString::asprintf("高度：%d  宽度：%d", src_img.rows, src_img.cols));

    ui->label_img->setPixmap(QPixmap::fromImage(QImage(src_img.data, src_img.cols, src_img.rows, src_img.step, QImage::Format_RGB888)));
    QApplication::processEvents();
    auto output_img = convert_to_pseudo_color(predict(src_img));
    cv::addWeighted(src_img, 0.6, output_img, 0.4, 0.0, output_img);
    ui->label_img->setPixmap(QPixmap::fromImage(QImage(output_img.data, output_img.cols, output_img.rows, output_img.step, QImage::Format_RGB888)));

}

void MainWindow::on_pushButton_1_clicked() {
    auto filename = QFileDialog::getOpenFileName();
    if (!filename.isEmpty()) {
        this->forward_by_fileneme(filename);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()){
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    this -> forward_by_fileneme(event->mimeData()->urls().first().toLocalFile());
}