#include <QCameraDevice>
#include <QMediaDevices>
#include <iostream>
#include <string>
#include <vector>

#include "./camera/list.hpp"

using Table = std::vector<std::vector<std::string>>;

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
std::string listCameras() {
  const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
  Table table = qlistToTable(cameras);
  return tableToString(table);
}
