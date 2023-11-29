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
  ms elapsed;
  ms duration;
  void stopAfterDuration();

 public:
  explicit Recorder(QVideoSink *sink, QObject *parent = nullptr);
  void save();
  void start(ms interval = ms(0), ms duration = ms(0),
             ms min_interval = ms(1000));
};
