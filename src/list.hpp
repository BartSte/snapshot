#include <QCameraDevice>
#include <QMediaDevices>
#include <string>
#include <vector>

using Table = std::vector<std::vector<std::string>>;

std::string listCameras();
Table qlistToTable(QList<QCameraDevice> cameras);
std::string tableToString(Table table, std::string separator = " | ");
/* std::vector<int> getColumnWidths(Table table); */
