#pragma once

#include <QThread>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <qobject.h>
#include <qtimer.h>
#include <qvideoframe.h>
#include <qvideosink.h>
#include <spdlog/spdlog.h>
#include <string>

#include "video/connect.hpp"

enum class RecorderState {
  Stop = 0,
  Start = 1,
};

class ImageSaver : public QObject {

  using path = std::filesystem::path;

  Q_OBJECT

 public slots:
  void save();

 public:
  ImageSaver(QVideoSink *sink, path subdirectory, QObject *parent = nullptr);

 private:
  QVideoSink *sink;
  path save_dir;
  void saveFrame(const QVideoFrame &frame);
};

class Recorder : public QObject {

  using ms = std::chrono::milliseconds;
  using path = std::filesystem::path;

  Q_OBJECT
  QThread worker;

 private:
  QTimer timer;
  ms elapsed;
  ms duration;
  std::unique_ptr<ImageSaver> saver;
  RecorderState state;
  void stopAfterDuration();
  bool isValidInterval(ms interval, ms min_interval);
  void applyDuration(ms duration);

 public:
  explicit Recorder(QVideoSink *sink, path save_path, QObject *p = nullptr);
  ~Recorder();
  void start(ms interval = ms(0), ms duration = ms(0),
             ms min_interval = ms(1000));
  void stop();
  RecorderState getState();
};
