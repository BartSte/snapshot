#pragma once

#include <QCameraDevice>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;
using boost::optional;

optional<QCameraDevice> selectCamera(const pt::ptree &config);
optional<QCameraDevice> selectCameraByName(const QList<QCameraDevice> &cameras,
                                           const std::string &name);
