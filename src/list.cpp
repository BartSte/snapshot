#include "./list.hpp"
#include <QCameraDevice>
#include <QMediaDevices>
#include <iostream>
#include <qcameradevice.h>
#include <string>
#include <vector>

/**
 * @brief VideoScene::getCameraDevice
 *
 * Uses the QMediaDevices::videoInputs() function to get a list of cameras.
 * This QList is then cast to a std::string. Next the std::string is formatted
 * such that it can easily be interpreted by the user.
 *
 * @return The available cameras as a std::string.
 */
std::string listCameras() {
  const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
  return qlistToString(cameras);
}

/**
 * @brief qlistToString
 *
 * Converts a QList of QCameraDevices to a std::string.
 *
 * @param cameras A QList of QCameraDevices.
 * @return the QCameraDevice objects as a std::string in the following format:
 * 0: <description of camera 0>
 * 1: <description of camera 1>
 * etc.
 */
std::string qlistToString(QList<QCameraDevice> cameras) {
  std::string result = "";
  for (int i = 0; i < cameras.size(); i++) {
    std::string description = cameras[i].description().toStdString();
    result += std::to_string(i) + ": " + description + "\n";
  }
  return result;
}
