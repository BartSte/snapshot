#include "./mainwindow.h"
#include "./ui_mainwindow.h"
#include "./viewer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), Ui::MainWindow(), viewer(this->graphicsView) {
  setupUi(this);
}

MainWindow::~MainWindow() {}
