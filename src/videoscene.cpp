#include "./videoscene.hpp"
#include <QCamera>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QMediaDevices>
#include <QWidget>
#include <iostream>
#include <qcameradevice.h>
#include <qobject.h>
#include <spdlog/spdlog.h>

const float constRatio = 0.8;
const float textOffset = 10;
const float textFontSize = 16;
const QString textFont = "Arial";

/**
 * @brief VideoScene::VideoScene
 *
 * @param mainWindow A pointer to a QWidget.
 */
VideoScene::VideoScene(QObject *parent)
    : QGraphicsScene(parent),
      camera(parent),
      pixmapItem(),
      session(parent),
      textItem(),
      videoItem() {
  setPixmap(":/disconnected.png");
  setText("No camera available");
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
  pixmapItem.setPixmap(pixmap);
  addItem(&pixmapItem);
}

/**
 * @brief VideoScene::resizeEvent
 * @param event The resize event.
 * Resize the QGraphicsPixmapItem and the QGraphicsTextItem when the scene is
 * resized.
 */
void VideoScene::scalePixmap() {
  QPixmap image = pixmapItem.pixmap();

  float widthRatio = sceneRect().width() / image.size().width();
  float heightRatio = sceneRect().height() / image.size().height();
  float aspectRatio = std::min(widthRatio, heightRatio);
  float clippedRatio = std::min(aspectRatio, 1.0f);

  pixmapItem.setScale(clippedRatio * constRatio);
}

/**
 * @brief VideoScene::centerPixmap
 * Center the QGraphicsPixmapItem in the scene.
 */
void VideoScene::centerPixmap() {
  float scale = pixmapItem.scale();
  QPixmap image = pixmapItem.pixmap();

  float widthImage = image.size().width() * scale;
  float heightImage = image.size().height() * scale;
  float x = (sceneRect().width() - widthImage) / 2;
  float y = (sceneRect().height() - heightImage) / 2;

  pixmapItem.setPos(x, y);
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

  textItem.setPlainText(qtext);
  textItem.setFont(font);
  addItem(&textItem);
}

/**
 * @brief VideoScene::positionText
 * Position the text below the QGraphicsPixmapItem, in the center of the scene.
 */
void VideoScene::centerText() {
  float scale = pixmapItem.scale();
  QPixmap image = pixmapItem.pixmap();

  float widthText = textItem.boundingRect().width();
  float heightImage = image.size().height() * scale;

  float x = (sceneRect().width() / 2) - (widthText / 2);
  float y = (sceneRect().height() - heightImage) / 2 + heightImage;

  textItem.setPos(x, y + textOffset);
}

/**
 * @brief TODO
 *
 * @param device
 */
void VideoScene::setVideo(const QCameraDevice &device) {
  pixmapItem.setVisible(false);
  textItem.setVisible(false);
  setCamera(device);
}

/**
 * @brief VideoScene::updateResolution
 * Select the highest resolution available for the camera.
 */
void VideoScene::updateResolution() {
  QList<QCameraFormat> formats = camera.cameraDevice().videoFormats();
  SPDLOG_DEBUG("formats: {}", formats.size());

  QSize max_reso(0, 0);
  QCameraFormat selected;
  for (QCameraFormat format : formats) {
    QSize reso = format.resolution();
    if (reso.width() > max_reso.width() && reso.height() > max_reso.height()) {
      max_reso = reso;
      selected = format;
      SPDLOG_DEBUG("resolution: {}x{}", reso.width(), reso.height());
    }
  }
  camera.setCameraFormat(selected);
}

/**
 * @brief VideoScene::scaleVideo
 * Scale the QGraphicsVideoItem while preserving the aspect ratio. The video
 * should never be cropped.
 */
void VideoScene::scaleVideo() {
  float widthRatio = sceneRect().width() / videoItem.size().width();
  float heightRatio = sceneRect().height() / videoItem.size().height();
  float aspectRatio = std::min(widthRatio, heightRatio);

  SPDLOG_DEBUG("aspectRatio: {}", aspectRatio);
  videoItem.setScale(aspectRatio);
}

/**
 * @brief VideoScene::centerVideo
 * Center the QGraphicsVideoItem in the scene.
 */
void VideoScene::centerVideo() {
  float scale = videoItem.scale();
  float widthVideo = videoItem.size().width() * scale;
  float heightVideo = videoItem.size().height() * scale;
  float x = (sceneRect().width() - widthVideo) / 2;
  float y = (sceneRect().height() - heightVideo) / 2;

  videoItem.setPos(x, y);
}

/**
 * @brief VideoScene::setCamera
 *
 * @param camera The camera to be displayed.
 *
 */
void VideoScene::setCamera(const QCameraDevice &device) {
  camera.setCameraDevice(device);
  session.setCamera(&camera);
  session.setVideoOutput(&videoItem);
  camera.start();
  addItem(&videoItem);
}

/**
 * @brief VideoScene::~VideoScene
 * Destructor.
 */
VideoScene::~VideoScene() { camera.stop(); }
