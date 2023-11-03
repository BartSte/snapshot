#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <QtMultimediaWidgets/qgraphicsvideoitem.h>
#include <qcamera.h>
#include <qcameradevice.h>
#include <qlist.h>
#include <qmediacapturesession.h>
#include <qmediadevices.h>
#include <qmediaplayer.h>
#include <qobject.h>
#include <qurl.h>
#include <spdlog/spdlog.h>
#include <string>

#include "./video/connect.hpp"
#include "./video/find.hpp"

using VideoPtr = std::optional<std::unique_ptr<Video>>;

// TODO: The Stream and File have overlapping code for the methods start(),
// stop(), setVideoOutput() and updateResolution(). In some sense, this is
// also true for the Camera class. I think I can move functionality to the base
// class.

/**
 * @brief convertState
 *
 * Convert a bool to a VideoState.
 *
 * @param active True if the state is active, false otherwise.
 * @return The VideoState.
 */
VideoState convertState(bool active) {
  if (active) {
    SPDLOG_INFO("VideoState::Started");
    return VideoState::Started;
  } else {
    SPDLOG_INFO("VideoState::Stopped");
    return VideoState::Stopped;
  }
}

/**
 * @brief convertState
 *
 * Convert the QMediaPlayer::PlaybackState to a VideoState.
 *
 * @param state The QMediaPlayer::PlaybackState.
 * @return The VideoState.
 */
VideoState convertState(QMediaPlayer::PlaybackState &state) {
  switch (state) {
  case QMediaPlayer::PlaybackState::StoppedState:
    SPDLOG_INFO("VideoState::Stopped");
    return VideoState::Stopped;
  case QMediaPlayer::PlaybackState::PlayingState:
    SPDLOG_INFO("VideoState::Started");
    return VideoState::Started;
  case QMediaPlayer::PlaybackState::PausedState:
    SPDLOG_INFO("VideoState::Paused");
    return VideoState::Paused;
  }
}

/**
 * @brief Constructor
 *
 * Inherits from QObject.
 *
 * @param parent The parent QObject. Default is nullptr.
 */
Video::Video(QObject *parent) : QObject(parent), state(VideoState::Stopped) {}

/**
 * @brief Constructor
 *
 * Create a stream from a url. The playback state of QMediaPlayer is connected
 * to the setState slot.
 *
 * @param url The url of the stream.
 * @param parent The parent QObject. Default is nullptr.
 */
Stream::Stream(const QUrl &url, QObject *parent) : Video(parent), player() {
  connect(&player, &QMediaPlayer::playbackStateChanged, this,
          &Stream::setState);
  player.setSource(url);
}

/**
 * @brief setState
 *
 * Convert the QMediaPlayer::PlaybackState to a VideoState and emit the
 * stateChanged signal if the state has changed.
 *
 * @param playerState The QMediaPlayer::PlaybackState.
 */
void Stream::setState(QMediaPlayer::PlaybackState playerState) {
  VideoState newState = convertState(playerState);

  if (newState != this->state) {
    this->state = newState;
    emit stateChanged();
  }
}

/**
 * @brief start
 *
 * Wrapper for QMediaPlayer::play().
 */
void Stream::start() { player.play(); }

/**
 * @brief stop
 *
 * Wrapper for QMediaPlayer::stop().
 */
void Stream::stop() { player.stop(); }

/**
 * @brief setVideoOutput
 *
 * Wrapper for QMediaPlayer::setVideoOutput().
 *
 * @param videoItem Pointer to the QGraphicsVideoItem.
 */
void Stream::setVideoOutput(QGraphicsVideoItem *videoItem) {
  player.setVideoOutput(videoItem);
}

/**
 * @brief constructor
 *
 * Connect the QMediaPlayer::playbackStateChanged signal to the setState slot.
 *
 * @param path The path to the file.
 * @param parent The parent QObject. Default is nullptr.
 */
File::File(const QString &path, QObject *parent) : Video(parent), player() {
  connect(&player, &QMediaPlayer::playbackStateChanged, this, &File::setState);
  player.setSource(QUrl::fromLocalFile(path));
}

/**
 * @brief setState
 *
 * Convert the QMediaPlayer::PlaybackState to a VideoState and emit the
 * stateChanged signal if the state has changed.
 *
 * @param playerState
 */
void File::setState(QMediaPlayer::PlaybackState playerState) {
  VideoState newState = convertState(playerState);

  if (newState != this->state) {
    this->state = newState;
    emit stateChanged();
  }
}

void File::start() { player.play(); }

void File::stop() { player.stop(); }

void File::setVideoOutput(QGraphicsVideoItem *videoItem) {
  player.setVideoOutput(videoItem);
}

/**
 * @brief constructor
 *
 * Connect the QCamera::activeChanged signal to the setState slot.
 *
 * @param device The QCameraDevice to use.
 * @param parent The parent QObject. Default is nullptr.
 */
Camera::Camera(const QCameraDevice &device, QObject *parent)
    : Video(), camera(), session() {
  connect(&camera, &QCamera::activeChanged, this, &Camera::setState);
  camera.setCameraDevice(device);
  session.setCamera(&camera);
}

void Camera::start() { camera.start(); }

void Camera::stop() { camera.stop(); }

void Camera::setState(bool active) {
  VideoState newState = convertState(active);

  if (newState != this->state) {
    this->state = newState;
    emit stateChanged();
  }
}

void Camera::setVideoOutput(QGraphicsVideoItem *videoItem) {
  session.setVideoOutput(videoItem);
}

/**
 * @brief updateResolution
 *
 * Update the resolution of the camera to the highest resolution available.
 */
void Camera::updateResolution() {
  QList<QCameraFormat> formats = camera.cameraDevice().videoFormats();
  SPDLOG_DEBUG("formats: {}", formats.size());

  QSize max_reso(0, 0);
  QCameraFormat selected;
  for (QCameraFormat format : formats) {
    QSize reso = format.resolution();
    if (reso.width() > max_reso.width() && reso.height() > max_reso.height()) {
      max_reso = reso;
      selected = format;
      SPDLOG_DEBUG("resolution: {}x{}", reso.width(), reso.height());
    }
  }
  camera.setCameraFormat(selected);
}

/**
 * @brief create
 *
 * Create a Video object from a string id. The id can be a url, a path to a
 * file or the name of a camera. If the id is not found, return std::nullopt.
 *
 * @param id The identifier of the video. This can be a url, a path to a file,
 * or the name of a camera.
 * @return A unique pointer to the Video object, or std::nullopt if the id is
 * not found.
 */
VideoPtr VideoFactory::create(const std::string &id) {
  QUrl url = findStream(id);
  QString path = findFile(id);
  QCameraDevice device = findCamera(id);

  if (url.isValid()) {
    return std::make_unique<Stream>(url);

  } else if (!path.isNull()) {
    return std::make_unique<File>(path);

  } else if (!device.isNull()) {
    return std::make_unique<Camera>(device);

  } else {
    SPDLOG_WARN("No camera found.");
    return std::nullopt;
  }
}
