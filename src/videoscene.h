#pragma once

#include <QCamera>
#include <QGraphicsPixmapItem>
#include <QGraphicsVideoItem>
#include <QMediaCaptureSession>
#include <boost/optional.hpp>
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
  QCamera *camera;
  QGraphicsPixmapItem *pixmapItem;
  QGraphicsTextItem *textItem;
  QGraphicsVideoItem *videoItem;
  QMediaCaptureSession *session;

  explicit VideoScene(QWidget *mainWindow);

  void setPixmap(std::string path);
  void setText(std::string text);
  void setVideo(const QCameraDevice &device);


  void setCamera(const QCameraDevice &device);
  boost::optional<const QCameraDevice> getCameraDevice();

  void scalePixmap();
  void centerPixmap();
  void setPosText();

  ~VideoScene();
};
