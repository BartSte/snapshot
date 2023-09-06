#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <QMainWindow>
#include <camera/find.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

#include "./gui/mainwindow.hpp"

const char *TITLE = "Snap shot machine";

/**
 * @brief DOCS:
 *
 * @param parent 
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), Ui::MainWindow(), scene(this) {

  setupUi(this);
  setWindowTitle(QString::fromStdString(TITLE));
  graphicsView->setGeometry(rect());
  graphicsView->setScene(&scene);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  QRect viewRect = graphicsView->rect();
  scene.setSceneRect(viewRect);
  scene.update();

  SPDLOG_INFO("View rect: {}x{}", viewRect.width(), viewRect.height());
}
