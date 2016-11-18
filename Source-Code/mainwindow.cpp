#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfiledialog.h>
#include <circleandrectangledetector.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_released()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.png *.xpm *.jpg)"));
    ui->lineEdit->setText(fileName);
    CircleAndRectangleDetector circleAndRectangleDetector(fileName.toStdString());
}
