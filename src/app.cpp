#include "app.h"
#include "mainwindow.h"

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent), Ui::MainWindow() {
    setupUi(this);
}

AppWindow::~AppWindow() {}
