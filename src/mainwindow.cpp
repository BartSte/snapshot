#include "./mainwindow.h"
#include "./videoscene.h"
#include <QMainWindow>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), Ui::MainWindow(), scene(new VideoScene(this)) {

  setupUi(this);
  graphicsView->setScene(scene);
}

MainWindow::~MainWindow() { delete scene; }
