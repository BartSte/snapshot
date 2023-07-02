#include "app.h"
#include "./mainwindow.h"

AppWindow::AppWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

AppWindow::~AppWindow()
{
    delete ui;
}

