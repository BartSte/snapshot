#include <QVideoSink>
#include <QtMultimediaWidgets/qgraphicsvideoitem.h>
#include <qcamera.h>
#include <qcameradevice.h>
#include <qlist.h>
#include <qmediacapturesession.h>
#include <qmediadevices.h>
#include <qmediaplayer.h>
#include <qmetaobject.h>
#include <qobject.h>
#include <qurl.h>
#include <qvideosink.h>
#include <spdlog/spdlog.h>
#include <string>

#include "./video/connect.hpp"
#include "./video/find.hpp"

using VideoPtr = std::optional<std::unique_ptr<BaseVideo>>;

/**
 * @brief Constructor
 *
 * Inherits from QObject. A QVideoSink is created and set as the video sink.
 * This is the QVideoSink object that can be used to display and record the
 * video, using a QGraphicsVideoItem and a Recorder object, respectively.
 *
 * @param parent The parent QObject. Default is nullptr.
 */
BaseVideo::BaseVideo(QObject *parent)
    : QObject(parent), sink(parent), state(VideoState::Stopped) {}

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
 * @brief Constructor
 *
 * Used to create a MediaPlayer from a url.
 *
 * @param url The url of the stream.
 * @param parent The parent QObject. Default is nullptr.
 */
MediaPlayer::MediaPlayer(const QUrl &url, QObject *parent)
    : BaseVideo(parent), player() {
  connect(&player, &QMediaPlayer::playbackStateChanged, this,
          &MediaPlayer::setState);
  player.setSource(url);
  setVideoSink(&sink);
}

/**
 * @brief Constructor
 *
 * Used to create a MediaPlayer from a path to a file.
 *
 * @param path The path to the file.
 * @param parent The parent QObject. Default is nullptr.
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

QVideoSink *MediaPlayer::getVideoSink() { return player.videoSink(); }

/**
 * @brief setVideoSink
 *
 * Adapter for QMediaPlayer::setVideoSink().
 *
 * @param sink a pointer to the QVideoSink.
 */
void MediaPlayer::setVideoSink(QVideoSink *sink_ptr) {
  player.setVideoSink(sink_ptr);
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
  setVideoSink(&sink);
}

void Camera::start() { camera.start(); }

void Camera::stop() { camera.stop(); }

void Camera::setState(bool active) {
  BaseVideo::setState(convertState(active));
}

QVideoSink *Camera::getVideoSink() { return session.videoSink(); }

/**
 * @brief setVideoSink
 *
 * Adapter for QMediaCaptureSession::setVideoSink().
 *
 * @param sink A pointer to the QVideoSink.
 */
void Camera::setVideoSink(QVideoSink *sink) { session.setVideoSink(sink); }

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
