#include <QObject>
#include <QVideoSink>

#include "./video/record.hpp"

Recorder::Recorder(QVideoSink *sink, QObject *parent) : QObject(parent) {
  connect(sink, &QVideoSink::videoFrameChanged, this, &Recorder::print);
}

void Recorder::print(const QVideoFrame &frame) {
  spdlog::info("Frame received");
}
