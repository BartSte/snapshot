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

// TODO:
// - Ensure it works with the streams as well
// - When a stream disconnects, the video state should change
// - The window must be displayed before trying to connect to the video.
// Showing the GUI cannot be blocked by starting the video

enum class VideoState {
  Stopped = 0,
  Paused = 1,
  Started = 2,
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
  void stateChanged();
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

  explicit Stream(const QUrl &url, QObject *parent = nullptr);
  ~Stream() override = default;
  void start() override;
  void stop() override;
  bool isNull() override;
  void setVideoOutput(QGraphicsVideoItem *videoItem) override;
  void updateResolution() override;

 protected:
  void setState(QMediaPlayer::PlaybackState state);
};

class File : public Video {

  Q_OBJECT

 public:
  QMediaPlayer player;

  explicit File(const QString &path, QObject *parent = nullptr);
  ~File() override = default;
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

  explicit Camera(const QCameraDevice &device, QObject *parent = nullptr);
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
