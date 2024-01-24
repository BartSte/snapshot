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
    : QObject(parent), sink(sink), save_dir(directory), bytesSaved(0) {}

/**
 * @brief save
 *
 * A slot to save a frame from the video sink.
 */
void ImageSaver::save() {
  if (!Path::mkdir(save_dir)) {
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
    spdlog::warn("Recorder received an invalid frame. Not saving.");
    return;
  }

  const QImage image = frame.toImage();
  const path file_path = save_dir / path(timestamp() + ".png");
  bool is_saved = image.save(file_path.string().c_str());

  if (is_saved) {
    addBytesSaved(image.sizeInBytes());
    emit frameSaved();
    spdlog::info("Saved frame to {}", file_path.string());
    spdlog::debug("Saved {} bytes in total.", bytesSaved);
  } else {
    spdlog::warn("Failed to save frame to {}", file_path.string());
  }
}

/**
 * @brief getBytesSaved
 *
 * Get the number of bytes saved.
 *
 * @return the number of bytes saved
 */
uint64_t ImageSaver::getBytesSaved() { return bytesSaved; }

/**
 * @brief addBytesSaved
 *
 * Add bytes to the number of bytes saved. UINT64_MAX is set as the value when
 * the number of bytes saved overflows.
 *
 * @param bytes the number of bytes to add
 */
void ImageSaver::addBytesSaved(uint64_t bytes) {
  if (bytesSaved + bytes < bytesSaved) {
    bytesSaved = UINT64_MAX;
  } else {
    bytesSaved += bytes;
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
      duration(0),
      maxBytes(UINT64_MAX) {
  if (!Path::mkdir(directory)) {
    spdlog::warn("Failed to create the base directory {}", directory.string());
  }

  saver.moveToThread(&worker);
  connect(&timer, &QTimer::timeout, &saver, &ImageSaver::save);
  connect(&timer, &QTimer::timeout, this, &Recorder::stopAfterDuration);
  connect(&saver, &ImageSaver::frameSaved, this, &Recorder::stopMaxBytes);
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
    stop();
    spdlog::info("Recording finished. The elapsed time is {} ms.",
                 elapsed.count());
  }
}

/**
 * @brief stopMaxBytes
 *
 * Stop recording after the maximum number of bytes has been saved.
 *
 */
void Recorder::stopMaxBytes() {
  if (saver.getBytesSaved() >= maxBytes) {
    spdlog::info("Recording finished. The maximum number of bytes is {}, and "
                 "the number of bytes saved is {}.",
                 maxBytes, saver.getBytesSaved());
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
void Recorder::start(ms interval, ms duration, ms min_interval,
                     uint64_t bytes) {
  if (!isValidInterval(interval, min_interval)) {
    return;
  }
  elapsed = ms(0);
  this->duration = duration;
  maxBytes = bytes;
  timer.start(interval);
  worker.start();
  spdlog::debug("Recorder is started");
  setState(RecorderState::Start);
}

/**
 * @brief stop
 *
 * Stop recording.
 */
void Recorder::stop() {
  timer.stop();
  worker.quit();
  worker.wait();
  spdlog::debug("Recorder is stopped");
  setState(RecorderState::Stop);
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

/**
 * @brief setState
 *
 * Set the state of the recorder.
 *
 * @param state the state of the recorder
 */
void Recorder::setState(RecorderState state) {
  if (state != this->state) {
    this->state = state;
    emit stateChanged();
  }
}
