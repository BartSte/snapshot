#include "./videoscene.h"
#include <QCamera>
#include <QGraphicsVideoItem>
#include <QMediaDevices>
#include <boost/optional.hpp>
#include <boost/optional/optional.hpp>
#include <cctype>
#include <iostream>
#include <qcamera.h>
#include <qgraphicsscene.h>
#include <qwidget.h>

const float constRatio = 0.8;
const float textOffset = 10;
const float textFontSize = 16;
const QString textFont = "Arial";

/**
 * @brief VideoScene::VideoScene
 *
 * @param mainWindow A pointer to a QWidget.
 */
VideoScene::VideoScene(QWidget *mainWindow)
    : QGraphicsScene(mainWindow),
      camera(new QCamera(mainWindow)),
      pixmapItem(new QGraphicsPixmapItem()),
      session(new QMediaCaptureSession(mainWindow)),
      textItem(new QGraphicsTextItem()),
      videoItem(new QGraphicsVideoItem()) {

  boost::optional<const QCameraDevice> device = getCameraDevice();
  if (device) {
    setCamera(device.get());
  } else {
    setPixmap(":/images/disconnected.png");
    setText("No camera available");
  }
}

/**
 * @brief VideoScene::setPixmap
 *
 * @param path The path to the image.
 * Set the image to be displayed in the scene.
 */
void VideoScene::setPixmap(std::string path) {
  QString qpath = QString::fromStdString(path);
  QPixmap pixmap(qpath);
  pixmapItem->setPixmap(pixmap);
  addItem(pixmapItem);
}

/**
 * @brief VideoScene::setText
 *
 * @param text The text to be displayed.
 * Set the text to be displayed in the scene.
 */
void VideoScene::setText(std::string text) {
  QString qtext = QString::fromStdString(text);
  QFont font(textFont, textFontSize);

  textItem->setPlainText(qtext);
  textItem->setFont(font);
  addItem(textItem);
}

/**
 * @brief VideoScene::resizeEvent
 * @param event The resize event.
 * Resize the QGraphicsPixmapItem and the QGraphicsTextItem when the scene is
 * resized.
 */
void VideoScene::scalePixmap() {
  QPixmap image = pixmapItem->pixmap();

  float widthRatio = sceneRect().width() / image.size().width();
  float heightRatio = sceneRect().height() / image.size().height();
  float aspectRatio = std::min(widthRatio, heightRatio);
  float clippedRatio = std::min(aspectRatio, 1.0f);

  pixmapItem->setScale(clippedRatio * constRatio);
}

/**
 * @brief VideoScene::centerPixmap
 * Center the QGraphicsPixmapItem in the scene.
 */
void VideoScene::centerPixmap() {
  float scale = pixmapItem->scale();
  QPixmap image = pixmapItem->pixmap();

  float widthImage = image.size().width() * scale;
  float heightImage = image.size().height() * scale;
  float x = (sceneRect().width() - widthImage) / 2;
  float y = (sceneRect().height() - heightImage) / 2;

  pixmapItem->setPos(x, y);
}

/**
 * @brief VideoScene::positionText
 * Position the text below the QGraphicsPixmapItem, in the center of the scene.
 */
void VideoScene::setPosText() {
  float scale = pixmapItem->scale();
  QPixmap image = pixmapItem->pixmap();

  float widthText = textItem->boundingRect().width();
  float heightImage = image.size().height() * scale;

  float x = (sceneRect().width() / 2) - (widthText / 2);
  float y = (sceneRect().height() - heightImage) / 2 + heightImage;

  textItem->setPos(x, y + textOffset);
}

/**
 * @brief TODO
 *
 * @param device 
 */
void VideoScene::setVideo(const QCameraDevice &device) {
  setCamera(device);
}

/**
 * @brief VideoScene::setCamera
 *
 * @param camera The camera to be displayed.
 *
 */
void VideoScene::setCamera(const QCameraDevice &device) {
  camera->setCameraDevice(device);
  session->setCamera(camera);
  session->setVideoOutput(videoItem);
  camera->start();
  addItem(videoItem);
}

/**
 * @brief VideoScene::getCameraDevice
 *
 * @return The first camera available.
 */
boost::optional<const QCameraDevice> VideoScene::getCameraDevice() {
  const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
  if (cameras.size() > 0) {
    return cameras[0];
  } else {
    return boost::none;
  }
}

/**
 * @brief VideoScene::~VideoScene
 * Destructor.
 */
VideoScene::~VideoScene() {
  delete pixmapItem;
  delete textItem;
  delete videoItem;
  delete session;
  delete camera;
}
