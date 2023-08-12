#pragma once

#include <QCameraDevice>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>

using boost::optional;

std::string listCameras();
std::string selectStream(const std::string &id);
optional<QCameraDevice> selectCamera(const std::string &id);
optional<QCameraDevice> selectCameraByName(const QList<QCameraDevice> &cameras,
                                           const std::string &name);
