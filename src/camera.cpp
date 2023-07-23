#include <QMediaDevices>
#include <QCameraDevice>
#include <boost/optional.hpp>
#include <boost/optional/optional.hpp>

boost::optional<QCameraDevice> selectCamera(QList<QCameraDevice> cameras) {
  if (cameras.size() > 0) {
    return cameras[0];
  } else {
    return boost::none;
  }
}
