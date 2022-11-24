//
// Created on 2022/11/24.
//

#ifndef IMGSEGQTDEMO_MAINWINDOW_H
#define IMGSEGQTDEMO_MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow: public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow() noexcept override;

private:
    Ui::MainWindow *ui;

public slots:
    void on_pushButton_1_clicked();
};
#endif //IMGSEGQTDEMO_MAINWINDOW_H
