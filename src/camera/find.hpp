#pragma once

#include <QCameraDevice>
#include <memory>
#include <qcameradevice.h>
#include <qmediadevices.h>
#include <qurl.h>
#include <string>

QUrl findStream(const std::string &id);
QCameraDevice findCamera(const std::string &id);
std::string listCameras(const QList<QCameraDevice> &cameras = QMediaDevices::videoInputs());
