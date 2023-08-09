#include <QMainWindow>
#include <QMediaDevices>
#include <boost/optional.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

#include "./gui/mainwindow.hpp"
#include "./gui/videoscene.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), Ui::MainWindow(), scene(this) {

  setupUi(this);
  graphicsView->setGeometry(rect());
  graphicsView->setScene(&scene);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  updateScene();
}

/**
 * @brief MainWindow::setVideo
 *
 * Set the camera device.
 *
 * @param cameraDevice The camera device
 */
void MainWindow::setVideo(const QCameraDevice &cameraDevice) {
  this->cameraDevice = cameraDevice;
  scene.setVideo(cameraDevice);
  updateScene();
}

/**
 * @brief MainWindow::setVideo
 *
 * Set the video stream.
 *
 * @param cameraDevice The camera device
 */
void MainWindow::setVideo(const QString &url) {
  this->url = url;
  scene.setVideo(url);
  updateScene();
}

/**
 * @brief MainWindow::updateScene
 *
 * Update the scene and its elements.
 */
void MainWindow::updateScene() {
  QRect viewRect = graphicsView->rect();
  SPDLOG_INFO("View rect: {}x{}", viewRect.width(), viewRect.height());

  scene.setSceneRect(viewRect);

  scene.scalePixmap();
  scene.centerPixmap();

  scene.centerText();

  scene.updateResolution();
  scene.scaleVideo();
  scene.centerVideo();
}

MainWindow::~MainWindow() {}
