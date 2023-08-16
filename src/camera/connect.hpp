#pragma once

#include <QtMultimediaWidgets/qgraphicsvideoitem.h>
#include <memory>
#include <qcamera.h>
#include <qcameradevice.h>
#include <qmediacapturesession.h>
#include <qmediaplayer.h>
#include <qurl.h>
#include <string>

class Video {

 public:
  virtual ~Video() = default;
  virtual void start() = 0;
  virtual bool isNull() = 0;
  virtual void setVideoOutput(QGraphicsVideoItem *videoItem) = 0;
  virtual void updateResolution() = 0;
};

class NullVideo : public Video {

 public:
  ~NullVideo() override = default;
  void start() override {}
  // TODO: remove isNull in favor of smart signals
  bool isNull() override { return true; }
  void setVideoOutput(QGraphicsVideoItem *videoItem) override {}
  void updateResolution() override {}
};

class Stream : public Video {

 public:
  QMediaPlayer player;

  explicit Stream(const QUrl &url);
  ~Stream() override = default;
  void start() override;
  bool isNull() override;
  void setVideoOutput(QGraphicsVideoItem *videoItem) override;
  void updateResolution() override;
};

class Camera : public Video {

 public:
  QCamera camera;
  QMediaCaptureSession session;

  explicit Camera(const QCameraDevice &device);
  ~Camera() override = default;
  void start() override;
  bool isNull() override;
  void setVideoOutput(QGraphicsVideoItem *videoItem) override;
  void updateResolution() override;
};

class VideoFactory {
 public:
  std::unique_ptr<Video> create(const std::string &id);
};
