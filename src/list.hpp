#include <QCameraDevice>
#include <QMediaDevices>

std::string listCameras();
std::string qlistToString(QList<QCameraDevice> cameras);
