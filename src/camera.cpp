#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <QCameraDevice>
#include <QMediaDevices>
#include <boost/optional.hpp>
#include <boost/optional/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <spdlog/spdlog.h>

#include "./camera.hpp"

namespace pt = boost::property_tree;
using boost::optional;

/**
 * @brief selectCameraByName
 *
 * Select a camera by name from a list of cameras. If the camera is not found,
 * return boost::none.
 *
 * @param cameras The list of cameras to select from.
 * @param name The name of the camera to select.
 * @return The selected camera.
 */
optional<QCameraDevice> selectCamera(const pt::ptree &config) {
  std::string name = config.get<std::string>("camera");
  QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
  SPDLOG_DEBUG("Camera name: {}", name);

  if (cameras.size() == 0) {
    SPDLOG_WARN("No cameras found");
    return boost::none;
  } else if (name == "default") {
    SPDLOG_INFO("Selecting default camera");
    return QMediaDevices::defaultVideoInput();
  } else {
    SPDLOG_INFO("Found {} cameras", cameras.size());
    return selectCameraByName(cameras, name);
  }
}

/**
 * @brief selectCameraByName
 *
 * Select a camera by name from a list of cameras. If the camera is not found,
 * return boost::none.
 *
 * @param cameras The list of cameras to select from.
 * @param name The name of the camera to select.
 * @return The selected camera.
 */
optional<QCameraDevice> selectCameraByName(const QList<QCameraDevice> &cameras,
                                           const std::string &name) {
  for (const QCameraDevice &camera : cameras) {
    SPDLOG_DEBUG("Found camera: {}", camera.description().toStdString());
    if (camera.description().toStdString() == name) {
      SPDLOG_INFO("Selected camera: {}", camera.description().toStdString());
      return camera;
    }
  }
  SPDLOG_WARN("Camera not found");
  return boost::none;
}
