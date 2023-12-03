#include <QCamera>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QMediaDevices>
#include <QWidget>
#include <algorithm>
#include <iostream>
#include <qcameradevice.h>
#include <qobject.h>
#include <spdlog/spdlog.h>
#include <video/connect.hpp>

#include "./gui/videoscene.hpp"

const QString textFont = "Arial";
const float constRatio = 0.8;
const float textFontSize = 16;
const float textOffset = 10;

/**
 * @brief VideoScene::VideoScene
 *
 * @param mainWindow A pointer to a QWidget.
 */
VideoScene::VideoScene(QObject *parent)
    : QGraphicsScene(parent), pixmapItem(), textItem(), videoItem() {
  addItem(&pixmapItem);
  addItem(&textItem);
  addItem(&videoItem);
}

/**
 * @brief VideoScene::setPixmap
 *
 * @param path The path to the image.
 * Set the image to be displayed in the scene.
 */
void VideoScene::setPixmap(const std::string &path) {
  QString qpath = QString::fromStdString(path);
  QPixmap pixmap(qpath);
  pixmapItem.setPixmap(pixmap);
}

/**
 * @brief VideoScene::updatePixmap
 * Update the QGraphicsPixmapItem when called.
 */
void VideoScene::updatePixmap() {
  scalePixmap();
  centerPixmap();
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
void VideoScene::setText(const std::string &text) {
  QString qtext = QString::fromStdString(text);
  QFont font(textFont, textFontSize);

  textItem.setPlainText(qtext);
  textItem.setFont(font);
}

/**
 * @brief VideoScene::updateText
 * Update the QGraphicsTextItem when called.
 */
void VideoScene::updateText() { centerText(); }

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
 * @brief getVideo
 *
 * Set the video stream to the given camera.
 *
 * @param video
 */
void VideoScene::setVideo(std::shared_ptr<BaseVideo> video_) {
  video = video_;
  video->setVideoSink(videoItem.videoSink());
  connect(video.get(), &BaseVideo::stateChanged, this, &VideoScene::update);
  video->start();
}

/**
 * @brief VideoScene::updateVideo
 * Update the QGraphicsVideoItem when called.
 */
void VideoScene::updateVideo() {
  if (video) {
    video->updateResolution();
    scaleVideo();
    centerVideo();
  }
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

  spdlog::debug("aspectRatio: {}", aspectRatio);
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
 * @brief update
 *
 * Update the all QGraphicsItems in the scene.
 */
void VideoScene::update() {
  updatePixmap();
  updateText();
  updateVideo();
}
