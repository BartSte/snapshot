#include <video/record.hpp>

#include <QVideoSink>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <helpers/path.hpp>
#include <helpers/time.hpp>
#include <qobject.h>
#include <qtimer.h>
#include <spdlog/spdlog.h>

using ms = std::chrono::milliseconds;
using path = std::filesystem::path;

/**
 * @brief ImageSaver
 *
 * A class to save images from a QVideoSink.
 *
 * @param sink A video sink to save images from
 * @param save_dir A directory to save images to
 * @param parent A optional parent QObject
 */
ImageSaver::ImageSaver(QVideoSink *sink, path directory, QObject *parent)
    : QObject(parent), sink(sink), save_dir(directory) {}

/**
 * @brief save
 *
 * A slot to save a frame from the video sink.
 */
void ImageSaver::save() {
  if (!mkdir(save_dir)) {
    spdlog::warn("Failed to create the subdirectory {}", save_dir.string());
    return;
  }

  if (sink) {
    saveFrame(sink->videoFrame());
  } else {
    spdlog::critical("QVideoSink is a nullptr.");
  }
}

/**
 * @brief saveFrame
 *
 * Save a frame to the save directory.
 *
 * @param frame the frame to save
 */
void ImageSaver::saveFrame(const QVideoFrame &frame) {
  if (!frame.isValid()) {
    spdlog::warn("Invalid frame");
    return;
  }

  const QImage image = frame.toImage();
  const path file_path = save_dir / path(timestamp() + ".png");
  bool is_saved = image.save(file_path.string().c_str());

  if (is_saved) {
    spdlog::info("Saved frame to {}", file_path.string());
  } else {
    spdlog::warn("Failed to save frame to {}", file_path.string());
  }
}

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
      timer(parent),
      directory(save_path / path(timestamp())),
      saver(sink, directory),
      elapsed(0),
      duration(0) {
  if (!mkdir(directory)) {
    spdlog::warn("Failed to create the base directory {}", directory.string());
  }

  saver.moveToThread(&worker);
  connect(&timer, &QTimer::timeout, &saver, &ImageSaver::save);
  connect(&timer, &QTimer::timeout, this, &Recorder::stopAfterDuration);
  worker.start();
}

Recorder::~Recorder() {
  worker.quit();
  worker.wait();
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
    stop();
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
  elapsed = ms(0);
  this->duration = duration;
  timer.start(interval);
  state = RecorderState::Start;
}

/**
 * @brief stop
 *
 * Stop recording.
 */
void Recorder::stop() {
  timer.stop();
  state = RecorderState::Stop;
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
    spdlog::warn("Interval is 0, recording no frames.");
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

/**
 * @brief getRecorderState
 *
 * Get the state of the recorder.
 *
 * @return the state of the recorder
 */
RecorderState Recorder::getState() { return state; }
