#pragma once

#include <QVideoSink>
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
#include <qvideosink.h>
#include <spdlog/spdlog.h>
#include <string>

enum class VideoState {
  Stop = 0,
  Search = 1,
  Start = 2,
};

const std::map<VideoState, std::string> videoStateString = {
    {VideoState::Stop, "Stop"},
    {VideoState::Search, "Search"},
    {VideoState::Start, "Start"}};

class BaseVideo : public QObject {
  Q_OBJECT

 signals:
  void stateChanged();

 private:
  VideoState state;
  QVideoSink sink; // TODO: refactor
  void setStart(const QVideoFrame frame);

 public:
  explicit BaseVideo(QObject *parent = nullptr);
  VideoState getState() { return state; }
  void setState(const VideoState &newState);
  void setInternalVideoSink();

  virtual ~BaseVideo() = default;
  virtual void updateResolution() {}
  virtual void start();
  virtual void stop();

  virtual QVideoSink *getVideoSink() = 0;
  virtual void setVideoSink(QVideoSink *sink) = 0;
};

class MediaPlayer : public BaseVideo {

 private:
  QMediaPlayer player;

 public:
  explicit MediaPlayer(const QUrl &url, QObject *parent = nullptr);
  explicit MediaPlayer(const QString &url, QObject *parent = nullptr);
  ~MediaPlayer() override = default;
  void start() override;
  void stop() override;
  QVideoSink *getVideoSink() override;
  void setVideoSink(QVideoSink *sink) override;
};

class Camera : public BaseVideo {
 private:
  QCamera camera;
  QMediaCaptureSession session;

 public:
  explicit Camera(const QCameraDevice &device, QObject *parent = nullptr);
  ~Camera() override = default;
  void start() override;
  void stop() override;
  QVideoSink *getVideoSink() override;
  void setVideoSink(QVideoSink *sink) override;
  void updateResolution() override;
};

std::optional<std::unique_ptr<BaseVideo>> videoFactory(const std::string &id);
