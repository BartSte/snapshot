#pragma once

#include <QCamera>
#include <QGraphicsPixmapItem>
#include <QGraphicsVideoItem>
#include <QMediaCaptureSession>
#include <qcameradevice.h>
#include <qgraphicsscene.h>
#include <qscopedpointer.h>
#include <qsize.h>
#include <string>

/**
 * @class VideoScene
 * @brief The VideoScene class is a QGraphicsScene that displays a video
 * stream. If the stream is disconnected, it displays a disconnected image with
 * a text.
 */
class VideoScene : public QGraphicsScene {

 public:
  QGraphicsTextItem *textItem;
  QGraphicsPixmapItem *pixmapItem;
  QGraphicsVideoItem *videoItem;
  QMediaCaptureSession session;
  QScopedPointer<QCamera> camera;

  explicit VideoScene(QWidget *mainWindow);

  void setPixmap(std::string path);
  void setText(std::string text);
  void setCamera(const QCameraDevice &device);
  const QCameraDevice &getCameraDevice();
  bool camerasAvailable();

  void scalePixmap();
  void centerPixmap();
  void setPosText();

  ~VideoScene();
};
