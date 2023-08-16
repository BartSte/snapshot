#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "gmock/gmock.h"
#include <boost/property_tree/ptree.hpp>
#include <camera/find.hpp>
#include <gtest/gtest.h>
#include <qcameradevice.h>
#include <qlist.h>
#include <qmediadevices.h>
#include <spdlog/spdlog.h>

namespace pt = boost::property_tree;

//TODO: mock QCameraDevice such that it's description can be set to a specific 
// value for testing purposes.

QList<QCameraDevice> getCameras() {
  QList<QCameraDevice> cameras;
  const QCameraDevice camera1;
  const QCameraDevice camera2;
  cameras.append(camera1);
  cameras.append(camera2);
  return cameras;
}

/**
 * @brief Tests the listCameras function.
 *
 */
TEST(findTest, listCameras) {
  QList<QCameraDevice> cameras = getCameras();
  std::string actual = listCameras(cameras);
  std::string expected = "Index | Name | ID | Is Default | \n"
                         "0     |      |    | false      | \n"
                         "1     |      |    | false      | \n";
  ASSERT_EQ(actual, expected);
}

TEST(findTest, findCamera) {
  std::string name = "default";
  pt::ptree config;
  config.put("camera", name);
  QList<QCameraDevice> cameras = getCameras();

  QCameraDevice camera = findCamera(name, cameras);

  SPDLOG_DEBUG("Camera name: {}", camera.description().toStdString());
  ASSERT_EQ(camera.description().toStdString(), "");
}

TEST(findTest, findStream) {
  std::string name = "rtsp://localhost:8554/test";
  pt::ptree config;
  config.put("camera", name);

  QUrl stream = findStream(name);

  SPDLOG_DEBUG("Stream: {}", stream.toString().toStdString());
  /* ASSERT_EQ(stream.toString(), name); */
}
