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

using VideoPtr = std::optional<std::unique_ptr<BaseVideo>>;

/**
 * @brief Constructor
 *
 * Inherits from QObject.
 *
 * @param parent The parent QObject. Default is nullptr.
 */
BaseVideo::BaseVideo(QObject *parent)
    : QObject(parent), state(VideoState::Stopped) {}

/**
 * @brief setState
 *
 * Set the state of the video and emit the stateChanged signal if the state has
 * changed.
 *
 * @param newState
 */
void BaseVideo::setState(const VideoState &newState) {
  if (state != newState) {
    state = newState;
    emit stateChanged();
  }
}

/**
 * @brief TODO
 *
 * @param url
 * @param parent
 */
MediaPlayer::MediaPlayer(const QUrl &url, QObject *parent)
    : BaseVideo(parent), player() {
  connect(&player, &QMediaPlayer::playbackStateChanged, this,
          &MediaPlayer::setState);
  player.setSource(url);
}

/**
 * @brief TODO
 *
 * @param path
 * @param parent
 */
MediaPlayer::MediaPlayer(const QString &path, QObject *parent)
    : MediaPlayer(QUrl::fromLocalFile(path), parent) {}

/**
 * @brief start
 *
 * Wrapper for QMediaPlayer::play().
 */
void MediaPlayer::start() { player.play(); }

/**
 * @brief stop
 *
 * Wrapper for QMediaPlayer::stop().
 */
void MediaPlayer::stop() { player.stop(); }

/**
 * @brief setVideoOutput
 *
 * Wrapper for QMediaPlayer::setVideoOutput().
 *
 * @param videoItem Pointer to the QGraphicsVideoItem.
 */
void MediaPlayer::setVideoOutput(QGraphicsVideoItem *videoItem) {
  player.setVideoOutput(videoItem);
}

/**
 * @brief setState
 *
 * Convert the QMediaPlayer::PlaybackState to a VideoState and emit the
 * stateChanged signal if the state has changed.
 *
 * @param playerState The QMediaPlayer::PlaybackState.
 */
void MediaPlayer::setState(const QMediaPlayer::PlaybackState &playerState) {
  BaseVideo::setState(convertState(playerState));
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
    : BaseVideo(), camera(), session() {
  connect(&camera, &QCamera::activeChanged, this, &Camera::setState);
  camera.setCameraDevice(device);
  session.setCamera(&camera);
}

void Camera::start() { camera.start(); }

void Camera::stop() { camera.stop(); }

void Camera::setState(bool active) {
  BaseVideo::setState(convertState(active));
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
  spdlog::debug("formats: {}", formats.size());

  QSize max_reso(0, 0);
  QCameraFormat selected;
  for (QCameraFormat format : formats) {
    QSize reso = format.resolution();
    if (reso.width() > max_reso.width() && reso.height() > max_reso.height()) {
      max_reso = reso;
      selected = format;
      spdlog::debug("resolution: {}x{}", reso.width(), reso.height());
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
VideoPtr videoFactory(const std::string &id) {
  QUrl url = findStream(id);
  if (url.isValid()) {
    spdlog::info("MediaPlayer for a stream created.");
    return std::make_unique<MediaPlayer>(url);
  }

  QString path = findFile(id);
  if (!path.isNull()) {
    spdlog::info("MediaPlayer for a file created.");
    return std::make_unique<MediaPlayer>(path);
  }

  QCameraDevice device = findCamera(id);
  if (!device.isNull()) {
    spdlog::info("Camera created.");
    return std::make_unique<Camera>(device);
  } else {
    spdlog::warn("No camera found.");
    return std::nullopt;
  }
}
