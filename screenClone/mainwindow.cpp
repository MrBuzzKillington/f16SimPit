#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <Windows.h>
#include "CaptureModule.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

