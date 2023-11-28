#pragma once

#include <cstdint>
#include <memory>
#include <qobject.h>
#include <qtimer.h>
#include <qvideoframe.h>
#include <qvideosink.h>
#include <spdlog/spdlog.h>
#include <string>

#include "video/connect.hpp"

class Recorder : public QObject {
  using ms = std::chrono::milliseconds;

  Q_OBJECT

 protected:
  QVideoSink *sink;
  QTimer timer;
  ms duration;
  ms interval;

 public:
  explicit Recorder(QVideoSink *sink, QObject *parent = nullptr);
  void save(const QVideoFrame &frame);
  void start();
};
