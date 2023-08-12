#pragma once

#include <QCamera>
#include <QCameraDevice>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QMediaCaptureSession>
#include <QMediaPlayer>
#include <qobject.h>
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

  // TODO: move these to libcamera
  // setVideo should only feed the videoItem into the setVideoOutput method
  QCamera camera;
  QMediaCaptureSession session;
  QMediaPlayer player;

  explicit VideoScene(QObject *parent = nullptr);

  void setPixmap(std::string path);
  void updatePixmap();

  void setText(std::string text);
  void updateText();

  void setVideo(const QCameraDevice &device);
  void setVideo(const QString &url);
  void updateVideo();

  virtual ~VideoScene();

 private:
  void scalePixmap();
  void centerPixmap();

  void centerText();

  void updateResolution();
  void scaleVideo();
  void centerVideo();
  void setCamera(const QCameraDevice &device);
  void setStream(const QString &url);
};
