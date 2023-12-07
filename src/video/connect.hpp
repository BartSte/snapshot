#pragma once

#include "helpers/time.hpp"
#include <QVideoSink>
#include <QtMultimediaWidgets/qgraphicsvideoitem.h>
#include <chrono>
#include <map>
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
  using ms = std::chrono::milliseconds;
  using sec = std::chrono::seconds;

  Q_OBJECT

 signals:
  void stateChanged();

 private slots:
  void setStart(const QVideoFrame frame);

 private:
  VideoState state;
  QVideoSink sink;
  ResetTimer stopTimer;

 public:
  explicit BaseVideo(ms connectTimeout = sec(30), QObject *parent = nullptr);
  VideoState getState() { return state; }
  void setState(const VideoState &newState);

  virtual ~BaseVideo() = default;
  virtual void updateResolution() {}
  virtual void start();
  virtual void stop();
  virtual QVideoSink *getVideoSink();
  virtual void setVideoSink(QVideoSink *sink) = 0;
};

class MediaPlayer : public BaseVideo {
  using ms = std::chrono::milliseconds;
  using sec = std::chrono::seconds;

 private:
  QMediaPlayer player;

 public:
  explicit MediaPlayer(const QUrl &url, ms connectTimeout = sec(30),
                       QObject *parent = nullptr);
  explicit MediaPlayer(const QString &path, ms connectTimeout = sec(30),
                       QObject *parent = nullptr);
  ~MediaPlayer() override = default;
  void start() override;
  void stop() override;
  QVideoSink *getVideoSink() override;
  void setVideoSink(QVideoSink *sink) override;
};

class Camera : public BaseVideo {
  using ms = std::chrono::milliseconds;
  using sec = std::chrono::seconds;

 private:
  QCamera camera;
  QMediaCaptureSession session;

 public:
  explicit Camera(const QCameraDevice &device, ms connectTimeout = sec(30),
                  QObject *parent = nullptr);
  ~Camera() override = default;
  void start() override;
  void stop() override;
  QVideoSink *getVideoSink() override;
  void setVideoSink(QVideoSink *sink) override;
  void updateResolution() override;
};

std::optional<std::unique_ptr<BaseVideo>> videoFactory(
    const std::string &id,
    std::chrono::milliseconds connectTimeout = std::chrono::seconds(30));
