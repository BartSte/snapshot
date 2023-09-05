#pragma once

#include <QtMultimediaWidgets/qgraphicsvideoitem.h>
#include <memory>
#include <qcamera.h>
#include <qcameradevice.h>
#include <qmediacapturesession.h>
#include <qmediaplayer.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <qurl.h>
#include <string>

enum class VideoState {
  Stopped,
  Started,
  Paused,
};

class Video : public QObject {

  Q_OBJECT

 public:
  Video(QObject *parent = nullptr);
  VideoState getState();

  virtual ~Video() = default;
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual bool isNull() = 0;
  virtual void setVideoOutput(QGraphicsVideoItem *videoItem) = 0;
  virtual void updateResolution() = 0;

 protected:
  VideoState state;

 signals:
  void stateChanged(VideoState state);
};

class NullVideo : public Video {

 public:
  ~NullVideo() override = default;
  void start() override {}
  void stop() override {}
  bool isNull() override { return true; }
  void setVideoOutput(QGraphicsVideoItem *videoItem) override {}
  void updateResolution() override {}

};

class Stream : public Video {

  Q_OBJECT

 public:
  QMediaPlayer player;

  Stream(const QUrl &url, QObject *parent = nullptr);
  ~Stream() override = default;
  void start() override;
  void stop() override;
  bool isNull() override;
  void setVideoOutput(QGraphicsVideoItem *videoItem) override;
  void updateResolution() override;

 protected:
  void setState(QMediaPlayer::PlaybackState state);
};

class Camera : public Video {

  Q_OBJECT

 public:
  QCamera camera;
  QMediaCaptureSession session;

  Camera(const QCameraDevice &device, QObject *parent = nullptr);
  ~Camera() override = default;
  void start() override;
  void stop() override;
  bool isNull() override;
  void setVideoOutput(QGraphicsVideoItem *videoItem) override;
  void updateResolution() override;

 protected:
  void setState(bool is_active);
};

class VideoFactory {
 public:
  std::unique_ptr<Video> create(const std::string &id);
};
