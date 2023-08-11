#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <boost/property_tree/ptree.hpp>
#include <camera/connector.hpp>
#include <gtest/gtest.h>
#include <qcameradevice.h>
#include <spdlog/spdlog.h>

namespace pt = boost::property_tree;
using boost::optional;

TEST(cameraTest, selectCameraByName) {
  pt::ptree config;
  config.put("camera", "default");
  optional<QCameraDevice> camera = selectCamera(config);
  SPDLOG_DEBUG("Camera name: {}", camera->description().toStdString());
}
