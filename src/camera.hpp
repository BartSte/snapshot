#pragma once

#include <QCameraDevice>
#include <boost/optional.hpp>

boost::optional<QCameraDevice> selectCamera();
