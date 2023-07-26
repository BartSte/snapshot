#include "./mainwindow.hpp"
#include "./camera.hpp"
#include "./videoscene.hpp"
#include <QMainWindow>
#include <QMediaDevices>
#include <boost/optional.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), Ui::MainWindow(), scene(this) {

  setupUi(this);
  QList<QCameraDevice> devices = QMediaDevices::videoInputs();
  boost::optional<const QCameraDevice> cameraDevice = selectCamera(devices);
  if (cameraDevice) {
    scene.setVideo(cameraDevice.get());
  }
  graphicsView->setGeometry(rect());
  graphicsView->setScene(&scene);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
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
