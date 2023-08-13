#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <QtMultimediaWidgets/qgraphicsvideoitem.h>
#include <qcamera.h>
#include <qcameradevice.h>
#include <qlist.h>
#include <qmediacapturesession.h>
#include <qmediadevices.h>
#include <qmediaplayer.h>
#include <qurl.h>
#include <spdlog/spdlog.h>
#include <string>

#include "./camera/connect.hpp"
#include "./camera/find.hpp"

// DOCS:
std::unique_ptr<Video> Video::factory(const std::string &id) {
  if (findStream(id)) {
    return std::make_unique<Stream>(id);

  } else if (findCamera(id)) {
    return std::make_unique<Camera>(id);

  } else {
    SPDLOG_WARN("No camera found.");
    return nullptr;
  }
}

Stream::Stream(const std::string &id) : Video(), player() { setVideoInput(id); }

void Stream::start() { player.play(); }

void Stream::setVideoInput(const std::string &name) {
  player.setSource(QUrl(QString::fromStdString(name)));
}

void Stream::setVideoOutput(QGraphicsVideoItem *videoItem) {
  player.setVideoOutput(videoItem);
}

void Stream::updateResolution() {}

Camera::Camera(const std::string &id) : Video(), camera(), session() {
  setVideoInput(id);
}

void Camera::start() { camera.start(); }

void Camera::setVideoInput(const std::string &name) {
  QCameraDevice device = getCameraDevice(name);
  camera.setCameraDevice(device);
  session.setCamera(&camera);
}

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
