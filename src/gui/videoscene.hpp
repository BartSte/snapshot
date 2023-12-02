#pragma once

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <memory>
#include <optional>
#include <string>
#include <video/connect.hpp>

class VideoScene : public QGraphicsScene {
 private:
  std::shared_ptr<BaseVideo> video;

  void updatePixmap();
  void scalePixmap();
  void centerPixmap();

  void updateText();
  void centerText();

  void updateVideo();
  void scaleVideo();
  void centerVideo();

  void updateResolution();

 public:
  QGraphicsTextItem textItem;
  QGraphicsVideoItem videoItem;
  QGraphicsPixmapItem pixmapItem;

  explicit VideoScene(QObject *parent = nullptr);

  void setPixmap(const std::string &path);
  void setText(const std::string &text);
  void setVideo(std::shared_ptr<BaseVideo> video_);
  void update();
  void print(const QVideoFrame &frame) { spdlog::info("Test"); }
};
