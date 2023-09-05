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

#include "./camera/connect.hpp"
#include "./camera/find.hpp"

// DOCS: add docstrings

Video::Video(QObject *parent) : QObject(parent), state(VideoState::Stopped) {}
VideoState Video::getState() { return state; }

Stream::Stream(const QUrl &url, QObject *parent) : Video(parent), player() {
  connect(&player, &QMediaPlayer::playbackStateChanged, this,
          &Stream::setState);
  player.setSource(url);
}

void Stream::setState(QMediaPlayer::PlaybackState state) {
  switch (state) {
  case QMediaPlayer::PlaybackState::StoppedState:
    this->state = VideoState::Stopped;
    break;
  case QMediaPlayer::PlaybackState::PlayingState:
    this->state = VideoState::Started;
    break;
  case QMediaPlayer::PlaybackState::PausedState:
    this->state = VideoState::Paused;
    break;
  }
}

void Stream::start() { player.play(); }

void Stream::stop() { player.stop(); }

bool Stream::isNull() { return false; }

void Stream::setVideoOutput(QGraphicsVideoItem *videoItem) {
  player.setVideoOutput(videoItem);
}

void Stream::updateResolution() {}

Camera::Camera(const QCameraDevice &device, QObject *parent)
    : Video(), camera(), session() {
  connect(&camera, &QCamera::activeChanged, this, &Camera::setState);
  camera.setCameraDevice(device);
  session.setCamera(&camera);
}

void Camera::start() { camera.start(); }

void Camera::stop() { camera.stop(); }

void Camera::setState(bool active) {
  if (active) {
    state = VideoState::Started;
  } else {
    state = VideoState::Stopped;
  }
}

bool Camera::isNull() { return false; }

void Camera::setVideoOutput(QGraphicsVideoItem *videoItem) {
  session.setVideoOutput(videoItem);
}

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

// DOCS:
std::unique_ptr<Video> VideoFactory::create(const std::string &id) {
  QUrl url = findStream(id);
  QCameraDevice device = findCamera(id);

  if (url.isValid()) {
    return std::make_unique<Stream>(url);

  } else if (!device.isNull()) {
    return std::make_unique<Camera>(device);

  } else {
    SPDLOG_WARN("No camera found.");
    return std::make_unique<NullVideo>();
  }
}
