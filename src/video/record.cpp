#include <QVideoSink>
#include <cstdint>
#include <qobject.h>

#include "./video/record.hpp"

// TODO: docstrings!

Recorder::Recorder(QVideoSink *sink, QObject *parent)
    : QObject(parent), sink(sink), timer(parent) {
  /* connect(&timer, &QTimer::timeout, this, &Recorder::save); */
}

void Recorder::save(const QVideoFrame &frame) { spdlog::info("Frame saved."); }

void Recorder::start() { timer.start(); }
