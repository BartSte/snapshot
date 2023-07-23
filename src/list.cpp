#include "./list.hpp"
#include <QCameraDevice>
#include <QMediaDevices>
#include <iostream>
#include <string>
#include <vector>

using Table = std::vector<std::vector<std::string>>;

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
  Table table = qlistToTable(cameras);
  return tableToString(table);
}

Table qlistToTable(QList<QCameraDevice> cameras) {
  std::vector<std::string> headers = {"Index", "id", "isDefault",
                                      "Description"};
  std::vector<std::vector<std::string>> rows;
  rows.push_back(headers);
  for (int i = 0; i < cameras.size(); i++) {
    QCameraDevice camera = cameras.at(i);
    std::vector<std::string> row;
    row.push_back(std::to_string(i));
    row.push_back(camera.id().toStdString());
    row.push_back(std::to_string(camera.isDefault()));
    row.push_back(camera.description().toStdString());
    rows.push_back(row);
  }
  return rows;
}
std::string tableToString(Table table) {
  std::string result = "";
  result += "\n";
  for (int i = 0; i < table.size(); i++) {
    std::vector<std::string> row = table.at(i);
    for (int j = 0; j < row.size(); j++) {
      result += row.at(j);
      result += "\t";
    }
    result += "\n";
  }
  return result;
}
