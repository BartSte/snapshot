#pragma once

#include <qobject.h>
#include <qvideoframe.h>
#include <qvideosink.h>
#include <spdlog/spdlog.h>

#include "video/connect.hpp"

class Recorder : public QObject {
  Q_OBJECT

 public:
  Recorder(QVideoSink *sink, QObject *parent = nullptr);
  void print(const QVideoFrame &frame);
};
