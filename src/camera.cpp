#include <QCameraDevice>
#include <QMediaDevices>
#include <boost/optional.hpp>
#include <boost/optional/optional.hpp>

// TODO(barts): this function should get an input from the --list command
boost::optional<QCameraDevice> selectCamera() {
  QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
  if (cameras.size() > 0) {
    return cameras[0];
  } else {
    return boost::none;
  }
}
