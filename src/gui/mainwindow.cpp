#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <QMainWindow>
#include <QMediaDevices>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <camera.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

#include "./gui/mainwindow.hpp"
#include "./gui/videoscene.hpp"

namespace pt = boost::property_tree;

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
 * @brief MainWindow::enableCamera
 *
 * Enable the camera device or the stream.
 */
void MainWindow::enableCamera(const std::string &id) {
  std::string cameraStream = selectStream(id);
  boost::optional<QCameraDevice> cameraDevice = selectCamera(id);

  if (cameraStream != "") {
    SPDLOG_INFO("Using stream: {}", cameraStream);
    setVideo(QString::fromStdString(cameraStream));

  } else if (cameraDevice) {
    SPDLOG_INFO("Using camera: {}", cameraDevice->description().toStdString());
    setVideo(*cameraDevice);

  } else {
    SPDLOG_WARN("No camera selected.");
  }
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
