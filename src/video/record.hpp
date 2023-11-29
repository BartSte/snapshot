#pragma once

#include <boost/filesystem/path.hpp>
#include <cstdint>
#include <memory>
#include <qobject.h>
#include <qtimer.h>
#include <qvideoframe.h>
#include <qvideosink.h>
#include <spdlog/spdlog.h>
#include <string>

#include "video/connect.hpp"
#include "video/image_saver.hpp"

class Recorder : public QObject {
  using ms = std::chrono::milliseconds;
  using path = boost::filesystem::path;

  Q_OBJECT

 protected:
  QVideoSink *sink;
  QTimer timer;
  ImageSaver image_saver;
  ms elapsed;
  ms duration;
  void stopAfterDuration();
  bool isValidInterval(ms interval, ms min_interval);
  void applyDuration(ms duration);

 public:
  explicit Recorder(QVideoSink *sink, path save_path, QObject *p = nullptr);
  void save();
  void start(ms interval = ms(0), ms duration = ms(0),
             ms min_interval = ms(1000));
};
