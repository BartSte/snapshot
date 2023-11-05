#include <boost/property_tree/ptree.hpp>
#include <video/find.hpp>
#include <gtest/gtest.h>
#include <qcameradevice.h>
#include <qlist.h>
#include <qmediadevices.h>
#include <spdlog/spdlog.h>

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

TEST(findTest, findCameraEmpty) {
  std::string name = "";
  QCameraDevice camera = findCamera(name, QList<QCameraDevice>());
  ASSERT_EQ(camera.description().toStdString(), "");
}

TEST(findTest, findCameraDefault) {
  std::string name = "default";
  QCameraDevice defaultCamera = QMediaDevices::defaultVideoInput();
  QList<QCameraDevice> cameras = getCameras();

  QCameraDevice camera = findCamera(name, cameras);

  std::string actual = camera.description().toStdString();
  std::string expected = defaultCamera.description().toStdString();
  ASSERT_EQ(actual, expected);
}

TEST(findTest, findCameraNonDefault) {
  std::string name = "non-default";
  QList<QCameraDevice> cameras = getCameras();

  QCameraDevice camera = findCamera(name, cameras);

  // camera does not exist, so an empty one is returned
  std::string expected = "";
  std::string actual = camera.description().toStdString();
  ASSERT_EQ(actual, expected);
}

TEST(findTest, findStream) {
  QUrl stream;
  std::string name;

  std::vector<std::vector<std::string>> testers({
      {"rtsp://localhost:8554/test", "rtsp://localhost:8554/test"},
      {"udp://123.456.789.1:8554/test", "udp://123.456.789.1:8554/test"},
      {"http://localhost:8554/test", "http://localhost:8554/test"},
      {"https://localhost:8554/test", "https://localhost:8554/test"},
      {"file:///home/user/test", ""},
  });
  for (auto name_vs_expected : testers) {
    name = name_vs_expected[0];
    stream = findStream(name);
    ASSERT_EQ(stream.toString().toStdString(), name_vs_expected[1]);
  }
}
