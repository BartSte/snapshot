#include <video/connect.hpp>

#include <chrono>
#include <qcamera.h>
#include <qcameradevice.h>
#include <qlist.h>
#include <qmediacapturesession.h>
#include <qmediadevices.h>
#include <qmediaplayer.h>
#include <qmetaobject.h>
#include <qobject.h>
#include <qurl.h>
#include <qvideoframe.h>
#include <qvideosink.h>
#include <spdlog/spdlog.h>
#include <string>
#include <video/find.hpp>

using VideoPtr = std::optional<std::unique_ptr<BaseVideo>>;
using ms = std::chrono::milliseconds;
using sec = std::chrono::seconds;

/**
 * @brief Constructor
 *
 * Inherits from QObject. A QVideoSink is created and set as the video sink.
 * This is the QVideoSink object that can be used to display and record the
 * video, using a QGraphicsVideoItem and a Recorder object, respectively.
 *
 * @param parent The parent QObject. Default is nullptr.
 */
BaseVideo::BaseVideo(ms connectTimeout, QObject *parent)
    : QObject(parent),
      stopTimer(connectTimeout),
      sink(this),
      state(VideoState::Stop) {
  connect(&stopTimer, &ResetTimer::timeout, this, &BaseVideo::stop);
}

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
    spdlog::debug("Video state changed to {}", videoStateString.at(state));
  }
}

/**
 * @brief Start the video.
 *
 * The state is set to Search until frames are received from the video source.
 */
void BaseVideo::start() {
  stopTimer.start();
  setState(VideoState::Search);
  connect(getVideoSink(), &QVideoSink::videoFrameChanged, this,
          &Camera::setStart);
  connect(getVideoSink(), &QVideoSink::videoFrameChanged, &stopTimer,
          &ResetTimer::reset);
}

/**
 * @brief Set the state to start.
 *
 * The state is set to Started when frames are received from the video source.
 * The `frame` parameter is not used.
 */
void BaseVideo::setStart(const QVideoFrame frame) {
  if (frame.isValid() && state == VideoState::Search) {
    setState(VideoState::Start);
  }
}

/**
 * @brief Stop the video.
 *
 * The state is set to Stop and the video source is disconnected.
 */
void BaseVideo::stop() {
  disconnect(getVideoSink(), &QVideoSink::videoFrameChanged, this,
             &Camera::setStart);
  disconnect(getVideoSink(), &QVideoSink::videoFrameChanged, &stopTimer,
             &ResetTimer::reset);
  setState(VideoState::Stop);
}

/**
 * @brief getVideoSink
 *
 * Returns the video sink of BaseVideo.
 *
 * @return A pointer to the QVideoSink of BaseVideo.
 */
QVideoSink *BaseVideo::getVideoSink() { return &sink; }

/**
 * @brief Constructor
 *
 * Used to create a MediaPlayer from a path to a file.
 *
 * @param path The path to the file.
 * @param parent The parent QObject. Default is nullptr.
 */
MediaPlayer::MediaPlayer(const QString &path, ms connectTimeout,
                         QObject *parent)
    : MediaPlayer(QUrl::fromLocalFile(path), connectTimeout, parent) {}

/**
 * @brief Constructor
 *
 * Used to create a MediaPlayer from a url. The video sink of BaseVideo is set
 * to the video sink of the MediaPlayer. This is done to ensure that the video
 * sink of the MediaPlayer is not a nullptr. It is possible that the video sink
 * of the MediaPlayer will change, but the video sink of BaseVideo will not.
 *
 * @param url The url of the stream.
 * @param parent The parent QObject. Default is nullptr.
 */
MediaPlayer::MediaPlayer(const QUrl &url, ms connectTimeout, QObject *parent)
    : BaseVideo(connectTimeout, parent), player() {
  player.setSource(url);
  setVideoSink(BaseVideo::getVideoSink());
}

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
 * @brief getVideoSink
 *
 * Returns the video sink of the MediaPlayer. This may be different from the
 * video sink of BaseVideo.
 *
 * @return
 */
QVideoSink *MediaPlayer::getVideoSink() { return player.videoSink(); }

/**
 * @brief start
 *
 * Wrapper for QMediaPlayer::play().
 */
void MediaPlayer::start() {
  BaseVideo::start();
  player.play();
}

/**
 * @brief stop
 *
 * Wrapper for QMediaPlayer::stop().
 */
void MediaPlayer::stop() {
  player.stop();
  BaseVideo::stop();
}

/**
 * @brief constructor
 *
 * Connect the camera to the QMediaCaptureSession. The video sink of BaseVideo
 * is set to the video sink of the QMediaCaptureSession. This is done to ensure
 * that the video sink of the QMediaCaptureSession is not a nullptr. It is
 * possible that the video sink of the QMediaCaptureSession will change, but
 * the video sink of BaseVideo will not.
 *
 * @param device The QCameraDevice to use.
 * @param parent The parent QObject. Default is nullptr.
 */
Camera::Camera(const QCameraDevice &device, ms connectTimeout, QObject *parent)
    : BaseVideo(), camera(), session() {
  camera.setCameraDevice(device);
  session.setCamera(&camera);
  setVideoSink(BaseVideo::getVideoSink());
}

/**
 * @brief setVideoSink
 *
 * Adapter for QMediaCaptureSession::setVideoSink().
 *
 * @param sink A pointer to the QVideoSink.
 */
void Camera::setVideoSink(QVideoSink *sink) { session.setVideoSink(sink); }

/**
 * @brief getVideoSink
 *
 * Wrapper for QMediaCaptureSession::videoSink().
 *
 * @return A pointer to the QVideoSink.
 */
QVideoSink *Camera::getVideoSink() { return session.videoSink(); }

/**
 * @brief start
 *
 * Wrapper for Camera::start().
 */
void Camera::start() {
  BaseVideo::start();
  camera.start();
}

/**
 * @brief stop
 *
 * Wrapper for Camera::stop().
 */
void Camera::stop() {
  camera.stop();
  BaseVideo::stop();
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
VideoPtr videoFactory(const std::string &id, ms connectTimeout) {
  QUrl url = findStream(id);
  if (url.isValid()) {
    spdlog::info("MediaPlayer for a stream created.");
    return std::make_unique<MediaPlayer>(url, connectTimeout);
  }

  QString path = findFile(id);
  if (!path.isNull()) {
    spdlog::info("MediaPlayer for a file created.");
    return std::make_unique<MediaPlayer>(path, connectTimeout);
  }

  QCameraDevice device = findCamera(id);
  if (!device.isNull()) {
    spdlog::info("Camera created.");
    return std::make_unique<Camera>(device, connectTimeout);
  } else {
    spdlog::info("No camera found.");
    return std::nullopt;
  }
}
