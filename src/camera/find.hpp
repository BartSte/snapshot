#pragma once

#include <QCameraDevice>
#include <memory>
#include <string>

bool findStream(const std::string &id);
bool findCamera(const std::string &id);
std::string listCameras();
QCameraDevice getCameraDevice(const std::string &name);
