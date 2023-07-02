#include "mainwindow.h"
#include "ui_mainwindow.h"

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent), Ui::MainWindow() {
    setupUi(this);
}

AppWindow::~AppWindow() {}
