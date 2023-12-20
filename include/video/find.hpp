#pragma once

#include <memory>
#include <qcameradevice.h>
#include <qlist.h>
#include <qmediadevices.h>
#include <qurl.h>
#include <string>

QUrl findStream(const std::string &id);

QCameraDevice
findCamera(const std::string &id,
           const QList<QCameraDevice> &cameras = QMediaDevices::videoInputs());

std::string
listCameras(const QList<QCameraDevice> &cameras = QMediaDevices::videoInputs());

QString findFile(const std::string &id);
