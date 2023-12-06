#include <QMainWindow>
#include <iostream>
#include <spdlog/spdlog.h>
#include <video/find.hpp>

#include "./gui/mainwindow.hpp"

const char *TITLE = "Snap shot machine";

/**
 * @brief Constructor
 *
 * Initialize the GUI.
 *
 * @param parent The parent QWidget. Default is nullptr.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), Ui::MainWindow(), scene(this) {

  setupUi(this);
  setWindowTitle(QString::fromStdString(TITLE));
  graphicsView->setGeometry(rect());
  graphicsView->setScene(&scene);
}

/**
 * @brief resizeEvent
 *
 * Resize the scene when the window is resized.
 *
 * @param event The resize event.
 */
void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  QRect viewRect = graphicsView->rect();
  scene.setSceneRect(viewRect);
  scene.update();

  spdlog::debug("View rect: {}x{}", viewRect.width(), viewRect.height());
}
