#include <QVideoSink>
#include <cstdint>
#include <qobject.h>
#include <qtimer.h>

#include "./video/record.hpp"

using ms = std::chrono::milliseconds;

Recorder::Recorder(QVideoSink *sink, QObject *parent)
    : QObject(parent), sink(sink), timer(parent) {
  connect(&timer, &QTimer::timeout, this, &Recorder::save);
}

void Recorder::save() { spdlog::info("Frame saved."); }

void Recorder::stopAfterDuration() {
  elapsed += timer.intervalAsDuration();
  if (elapsed >= duration) {
    spdlog::info("Recording finished. The elapsed time is {} ms.",
                 elapsed.count());
    timer.stop();
  }
}

/**
 * @brief start
 *
 * Start recording.
 *
 * @param duration time to record for
 * @param interval time between frames
 * @param min_interval minimum time between frames
 */
void Recorder::start(ms interval, ms duration, ms min_interval) {
  if (interval.count() == 0) {
    spdlog::info("Interval is 0, recording no frames.");

  } else if (interval < min_interval) {
    spdlog::warn(
        "Interval is less than the minimum interval of {} ms. Recording is "
        "not started.",
        min_interval.count());

  } else {
    this->duration = duration;
    if (duration.count() == 0) {
      spdlog::info("Recording indefinitely.");
    } else {
      spdlog::info("Recording for {} ms.", duration.count());
      connect(&timer, &QTimer::timeout, this, &Recorder::stopAfterDuration);
    }
    timer.start(interval);
  }
}
