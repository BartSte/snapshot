#include <QVideoSink>
#include <chrono>
#include <cstdint>
#include <qobject.h>
#include <filesystem>
#include <qtimer.h>

#include "./video/record.hpp"

using ms = std::chrono::milliseconds;
using path = std::filesystem::path;

/**
 * @brief Recorder
 *
 * Create a new recorder.
 *
 * @param sink a video sink to record from
 * @param parent a optional parent QObject
 */
Recorder::Recorder(QVideoSink *sink, path save_path, QObject *parent)
    : QObject(parent),
      sink(sink),
      timer(parent),
      elapsed(0),
      duration(0),
      image_saver(save_path) {
  connect(&timer, &QTimer::timeout, this, &Recorder::save);
  connect(&timer, &QTimer::timeout, this, &Recorder::stopAfterDuration);
}

void Recorder::save() {
  image_saver.save();
}

/**
 * @brief stopAfterDuration
 *
 * Stop recording after the duration has elapsed. If the duration is 0, the
 * timer is never stopped.
 */
void Recorder::stopAfterDuration() {
  elapsed += timer.intervalAsDuration();
  if (elapsed >= duration && duration.count() != 0) {
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
  if (!isValidInterval(interval, min_interval)) {
    return;
  }
  this->duration = duration;
  timer.start(interval);
}

/**
 * @brief isValidInterval
 *
 * Check if the interval is valid. If the interval is 0, the recording is
 *
 *
 * @param interval
 * @param min_interval
 * @return
 */
bool Recorder::isValidInterval(ms interval, ms min_interval) {
  if (interval.count() == 0) {
    spdlog::info("Interval is 0, recording no frames.");
    return false;

  } else if (interval < min_interval) {
    spdlog::warn(
        "Interval is less than the minimum interval of {} ms. Recording is "
        "not started.",
        min_interval.count());
    return false;

  } else {
    return true;
  }
}
