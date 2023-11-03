#pragma once

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <video/connect.hpp>
#include <memory>
#include <string>

/**
 * @class VideoScene
 * @brief The VideoScene class is a QGraphicsScene that displays a video
 * stream. If the stream is disconnected, it displays a disconnected image with
 * a text.
 */
class VideoScene : public QGraphicsScene {

 public:
  QGraphicsTextItem textItem;
  QGraphicsVideoItem videoItem;
  QGraphicsPixmapItem pixmapItem;
  std::unique_ptr<Video> video;

  explicit VideoScene(QObject *parent = nullptr);

  void setPixmap(std::string path);
  void setText(std::string text);
  void setVideo(const std::string &id);
  void update();

 private:
  void updatePixmap();
  void updateText();
  void updateVideo();

  void scalePixmap();
  void centerPixmap();

  void centerText();

  void updateResolution();
  void scaleVideo();
  void centerVideo();
};
