#include <QCameraDevice>
#include <QFile>
#include <QMediaDevices>
#include <iostream>
#include <memory>
#include <qcameradevice.h>
#include <qurl.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

#include "./video/find.hpp"

using Table = std::vector<std::vector<std::string>>;

const char *STREAMS[4] = {"rtsp://", "udp://", "http://", "https://"};

/**
 * @brief findCameraByName
 *
 * Select a camera by name from a list of cameras. If the camera is not found,
 * an empty QCameraDevice is returned.
 *
 * @param cameras The list of cameras to select from.
 * @param name The name of the camera to select.
 * @return The selected camera.
 */
QCameraDevice findCameraByName(const QList<QCameraDevice> &cameras,
                               const std::string &name) {
  for (const QCameraDevice &camera : cameras) {
    spdlog::debug("Found camera: {}", camera.description().toStdString());
    if (camera.description().toStdString() == name) {
      spdlog::info("Selected camera: {}", camera.description().toStdString());
      return camera;
    }
  }
  spdlog::warn("Camera not found");
  return QCameraDevice();
}

/**
 * @brief findCamera
 *
 * Select a camera by name from a list of cameras. If the camera is not found,
 * an empty QCameraDevice is returned.
 *
 * @param cameras The list of cameras to select from.
 * @param name The name of the camera to select.
 * @return The selected camera.
 */
QCameraDevice findCamera(const std::string &name,
                         const QList<QCameraDevice> &cameras) {
  spdlog::debug("Camera name: {}", name);
  if (cameras.size() == 0) {
    spdlog::warn("No cameras found");
    return QCameraDevice();

  } else if (name == "default") {
    spdlog::info("Selecting default camera");
    return QMediaDevices::defaultVideoInput();

  } else {
    spdlog::info("Found {} cameras", cameras.size());
    return findCameraByName(cameras, name);
  }
}

/**
 * @brief findStream
 *
 * Select a camera stream from the config. If the stream is not found, return
 * and empty QUrl.
 *
 * @param config The config to select from.
 * @return The selected stream.
 */
QUrl findStream(const std::string &url) {
  for (const std::string &stream : STREAMS) {
    spdlog::debug("Compare potential stream {} to {}", stream, url);
    if (url.find(stream) != std::string::npos) {
      spdlog::info("Valid stream found: {}", url);
      return QUrl(QString::fromStdString(url));
    }
  }

  spdlog::info("No valid stream found.");
  return QUrl();
}

/**
 * @brief getCameraDevice
 *
 * Select a camera by name from a list of cameras. If the camera is not found,
 * an empty QCameraDevice is returned.
 *
 * @param string The name of the camera to select.
 * @return The selected camera.
 */
QCameraDevice getCameraDevice(const std::string &name) {
  QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
  for (const QCameraDevice &camera : cameras) {
    spdlog::debug("Found camera: {}", camera.description().toStdString());
    if (camera.description().toStdString() == name) {
      spdlog::info("Selected camera: {}", camera.description().toStdString());
      return camera;
    }
  }
  spdlog::warn("Camera not found");
  return QCameraDevice();
}

/**
 * @brief getColumnWidths
 *
 * Determines the width of each column in a Table. The width of each column is
 * determined by the longest string in that column.
 *
 * @param table The table to determine the column widths of.
 * @return A vector containing the width of each column.
 **/
std::vector<int> getColumnWidths(Table table) {
  std::vector<int> columnWidths;
  for (int i = 0; i < table.at(0).size(); i++) {
    int columnWidth = 0;
    for (int j = 0; j < table.size(); j++) {
      std::string cell = table.at(j).at(i);
      if (cell.length() > columnWidth) {
        columnWidth = cell.length();
      }
    }
    columnWidths.push_back(columnWidth);
  }
  return columnWidths;
}

/**
 * @brief tableToString
 *
 * Converts a Table to a std::string. The width of each column is determined by
 * the longest string in that column.
 *
 * @param table The table to convert.
 * @return The table as a std::string.
 **/
std::string tableToString(Table table, std::string separator = " | ") {
  std::string result = "";
  std::vector<int> columnWidths = getColumnWidths(table);
  for (int i = 0; i < table.size(); i++) {
    std::vector<std::string> row = table.at(i);
    for (int j = 0; j < row.size(); j++) {
      std::string cell = row.at(j);
      result += cell;
      result += std::string(columnWidths.at(j) - cell.length(), ' ');
      result += separator;
    }
    result += "\n";
  }
  return result;
}

/**
 * @brief qlistToTable
 *
 * Converts a QList of QCameraDevices to a Table. The first row of the Table
 * contains the headers of the table. The remaining rows contain the data.
 *
 * @param cameras The QList to convert.
 * @return The QList as a Table.
 **/
Table qlistToTable(QList<QCameraDevice> cameras) {
  std::vector<std::string> headers = {"Index", "Name", "ID", "Is Default"};
  std::vector<std::vector<std::string>> rows;
  rows.push_back(headers);
  for (int i = 0; i < cameras.size(); i++) {
    QCameraDevice camera = cameras.at(i);
    std::vector<std::string> row;
    row.push_back(std::to_string(i));
    row.push_back(camera.description().toStdString());
    row.push_back(camera.id().toStdString());
    row.push_back(camera.isDefault() ? "true" : "false");
    rows.push_back(row);
  }
  return rows;
}

/**
 * @brief VideoScene::getCameraDevice
 *
 * Uses the QMediaDevices::videoInputs() function to get a list of cameras.
 * This QList is then cast to a std::string. Next the std::string is formatted
 * such that it can easily be interpreted by the user.
 *
 * @return The available cameras as a std::string.
 */
std::string listCameras(const QList<QCameraDevice> &cameras) {
  Table table = qlistToTable(cameras);
  return tableToString(table);
}

/**
 * @brief findFile
 *
 * Select a file from the config. If the file is not found, return and empty
 * QString.
 *
 * @param config The config to select from.
 * @return The selected file.
 */
QString findFile(const std::string &path) {
  bool isFile = QFile::exists(QString::fromStdString(path));
  if (isFile) {
    spdlog::info("File found: {}", path);
    return QString::fromStdString(path);
  } else {
    spdlog::info("No file found.");
    return QString();
  }
}
