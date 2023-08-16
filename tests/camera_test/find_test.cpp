#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <boost/property_tree/ptree.hpp>
#include <camera/find.hpp>
#include <gtest/gtest.h>
#include <qcameradevice.h>
#include <qmediadevices.h>
#include <spdlog/spdlog.h>

namespace pt = boost::property_tree;

/**
 * @brief Tests the listCameras function.
 *
 */
TEST(findTest, listCameras) {
  QList<QCameraDevice> cameras;
  QCameraDevice camera1;
  QCameraDevice camera2;
  cameras.append(camera1);
  cameras.append(camera2);

  std::string actual = listCameras(cameras);
  std::string expected = "Index | Name | ID | Is Default | \n"
                         "0     |      |    | false      | \n"
                         "1     |      |    | false      | \n";
  ASSERT_EQ(actual, expected);
}

TEST(findTest, findCamera) {
  pt::ptree config;
  config.put("camera", "default");
  QCameraDevice camera = findCamera(config.get<std::string>("camera"));
  SPDLOG_DEBUG("Camera name: {}", camera.description().toStdString());
}
