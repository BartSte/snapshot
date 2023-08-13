#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <boost/property_tree/ptree.hpp>
#include <camera/find.hpp>
#include <gtest/gtest.h>
#include <qcameradevice.h>
#include <spdlog/spdlog.h>

namespace pt = boost::property_tree;
using boost::optional;

TEST(cameraTest, findCamera) {
  pt::ptree config;
  config.put("camera", "default");
  optional<QCameraDevice> camera = findCamera(config);
  SPDLOG_DEBUG("Camera name: {}", camera->description().toStdString());
}
