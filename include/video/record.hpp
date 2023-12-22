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
#include <video/connect.hpp>

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
  uint64_t getBytesSaved();

 private:
  QVideoSink *sink;
  path save_dir;
  uint64_t bytesSaved;
  void saveFrame(const QVideoFrame &frame);
  void addBytesSaved(uint64_t bytes);
};

class Recorder : public QObject {

  using ms = std::chrono::milliseconds;
  using path = std::filesystem::path;

  Q_OBJECT
  QThread worker;

 private:
  QTimer timer;
  path directory;
  ImageSaver saver;
  ms elapsed;
  ms duration;
  uint64_t maxBytes;
  RecorderState state;
  bool isValidInterval(ms interval, ms min_interval);
  void applyDuration(ms duration);

 private slots:
  void stopAfterDuration();
  void stopMaxBytes();

 public:
  explicit Recorder(QVideoSink *sink, path save_path, QObject *p = nullptr);
  ~Recorder();
  void start(ms interval = ms(0), ms duration = ms(0),
             ms min_interval = ms(1000), uint64_t max_bytes = UINT64_MAX);
  void stop();
  RecorderState getState();
};
