#include "./mainwindow.h"
#include "./videoscene.h"
#include <QMainWindow>
#include <QMediaDevices>
#include <boost/optional.hpp>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), Ui::MainWindow(), scene(this) {

  setupUi(this);
  boost::optional<const QCameraDevice> cameraDevice = getCameraDevice();
  if (cameraDevice) {
    scene.setVideo(cameraDevice.get());
  }
  graphicsView->setScene(&scene);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);

  QRect viewRect = graphicsView->rect();

  scene.setSceneRect(viewRect);

  scene.scalePixmap();
  scene.centerPixmap();

  scene.centerText();

  scene.scaleVideo();
  scene.centerVideo();
}

/**
 * @brief VideoScene::getCameraDevice
 *
 * @return The first camera available.
 */
boost::optional<const QCameraDevice> getCameraDevice() {
  const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
  if (cameras.size() > 0) {
    return cameras[0];
  } else {
    return boost::none;
  }
}
