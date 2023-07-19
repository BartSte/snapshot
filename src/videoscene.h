#pragma once

#include <QCamera>
#include <QCameraDevice>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QMediaCaptureSession>
#include <string>

/**
 * @class VideoScene
 * @brief The VideoScene class is a QGraphicsScene that displays a video
 * stream. If the stream is disconnected, it displays a disconnected image with
 * a text.
 */
class VideoScene : public QGraphicsScene {

 public:
  QCamera camera;
  QGraphicsTextItem textItem;
  QGraphicsVideoItem videoItem;
  QGraphicsPixmapItem pixmapItem;
  QMediaCaptureSession session;

  explicit VideoScene(QWidget *mainWindow);

  void setPixmap(std::string path);
  void scalePixmap();
  void centerPixmap();

  void setText(std::string text);
  void centerText();

  void setVideo(const QCameraDevice &device);
  /* void scaleVideo(); */
  /* void centerVideo(); */

  virtual ~VideoScene();

 private:
  void setCamera(const QCameraDevice &device);
};
