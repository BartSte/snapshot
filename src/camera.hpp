#pragma once

#include <QCameraDevice>
#include <boost/optional.hpp>

boost::optional<const QCameraDevice> selectCamera(QList<QCameraDevice> cameras);
