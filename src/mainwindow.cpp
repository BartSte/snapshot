#include "./mainwindow.h"
#include "./videoscene.h"
#include <QMainWindow>
#include <iostream>
#include <qnamespace.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), Ui::MainWindow(), scene(new VideoScene(this)) {

  setupUi(this);
  graphicsView->setScene(scene);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);

  QRect viewRect = graphicsView->rect();

  scene->setSceneRect(viewRect);
  scene->scalePixmap();
  scene->centerPixmap();
  scene->setPosText();
}

MainWindow::~MainWindow() { delete scene; }
