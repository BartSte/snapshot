#include "./mainwindow.h"
#include "./ui_mainwindow.h"
#include "./videoscene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), Ui::MainWindow(), scene(new VideoScene(this)) {

  setupUi(this);
  graphicsView->setScene(scene);

}

MainWindow::~MainWindow() {
  delete scene;
}
