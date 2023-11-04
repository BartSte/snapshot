#pragma once

#include <QtMultimediaWidgets/qgraphicsvideoitem.h>
#include <memory>
#include <optional>
#include <qcamera.h>
#include <qcameradevice.h>
#include <qmediacapturesession.h>
#include <qmediaplayer.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <qurl.h>
#include <string>

#include "./video/state.hpp"

class BaseVideo : public QObject {
  Q_OBJECT

 signals:
  void stateChanged();

 protected:
  VideoState state;
  explicit BaseVideo(QObject *parent = nullptr);
  void setState(const VideoState &newState);

 public:
  VideoState getState() { return state; }
  virtual ~BaseVideo() = default;
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void setVideoOutput(QGraphicsVideoItem *videoItem) = 0;
  virtual void updateResolution() {}
};

class MediaPlayer : public BaseVideo {
 protected:
  QMediaPlayer player;
  void setState(const QMediaPlayer::PlaybackState &state);

 public:
  explicit MediaPlayer(const QUrl &url, QObject *parent = nullptr);
  explicit MediaPlayer(const QString &url, QObject *parent = nullptr);
  ~MediaPlayer() override = default;
  void start() override;
  void stop() override;
  void setVideoOutput(QGraphicsVideoItem *videoItem) override;
};

class Camera : public BaseVideo {
 protected:
  QCamera camera;
  QMediaCaptureSession session;
  void setState(bool is_active);

 public:
  explicit Camera(const QCameraDevice &device, QObject *parent = nullptr);
  ~Camera() override = default;
  void start() override;
  void stop() override;
  void setVideoOutput(QGraphicsVideoItem *videoItem) override;
  void updateResolution() override;
};

std::optional<std::unique_ptr<BaseVideo>> videoFactory(const std::string &id);
