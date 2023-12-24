#include "helpers/path.hpp"
#include <QTest>
#include <cstdio>
#include <gtest/gtest.h>
#include <helpers/charpointers.hpp>
#include <helpers/coutredirect.hpp>
#include <qobject.h>
#include <qtestcase.h>
#include <snapshotapp.hpp>
#include <string>
#include <vector>
#include <video/connect.hpp>

using path = std::filesystem::path;

class GTestApp : public testing::Test {
 protected:
  std::stringstream buffer;
  CoutRedirect redirect{buffer.rdbuf()};
};

/**
 * @brief getArgv
 *
 * Returns an Argv object with the given vector of strings as arguments. The
 * first argument is 'test_app' and the second is '--config' with the path to
 * the config file. The rest of the arguments are the strings in the vector.
 *
 * @param vec vector of strings to be used as arguments
 * @return Argv object
 */
CharPointers getArgv(std::vector<std::string> vec = {}) {
  const path config = App::static_dir / "config_test.json";
  std::vector<std::string> strings = {"test_app", "--config", config.string()};
  CharPointers argv(strings);
  for (auto &str : vec) {
    argv.push_back(str);
  }
  return argv;
}

TEST(testApp, testGetArgv) {
  CharPointers argv = getArgv({"foo", "bar"});
  const path config = App::static_dir / "config_test.json";
  spdlog::info("config: {}", config.string());
  EXPECT_EQ(argv.size(), 5);
  EXPECT_STREQ(argv.data()[0], "test_app");
  EXPECT_STREQ(argv.data()[1], "--config");
  EXPECT_STREQ(argv.data()[2], config.string().c_str());
  EXPECT_STREQ(argv.data()[3], "foo");
  EXPECT_STREQ(argv.data()[4], "bar");
}

TEST_F(GTestApp, testPrintHelp) {
  CharPointers argv = getArgv({"--help"});
  App app(argv.size(), argv.data());
  std::cout << app.run();

  EXPECT_TRUE(buffer.str().find("Usage:") != std::string::npos);
}

TEST_F(GTestApp, testList) {
  CharPointers argv = getArgv({"--list"});
  App app(argv.size(), argv.data());
  std::cout << app.run();

  for (auto &column : {"Index", "Name", "ID", "Is Default"}) {
    EXPECT_TRUE(buffer.str().find(column) != std::string::npos);
  }
}

class TestCamera : public QObject {
  Q_OBJECT

 private:
  int exitCode;
  path tmpDir;

 private slots:
  void init() {
    exitCode = 0;
    tmpDir = std::filesystem::temp_directory_path() / "test_snapshot";
  }

  void cleanup() {
    std::filesystem::remove_all(tmpDir);
    QCOMPARE(exitCode, 0);
  }

  /**
   * @brief Test the --camera argument
   *
   * Normally, the --camera does not start the event loop. For testing, qWait
   * is used to run the event loop.
   */
  void testCamera() {
    CharPointers argv = getArgv({"--camera", App::debug_video.string()});
    App app(argv.size(), argv.data());
    int exitCode = app.run();
    QTest::qWait(2000);
  }

  /**
   * @brief Test the --record argument
   *
   * Normally, the --record starts the event loop. For testing, the event loope
   * is not started. Instead, qWait is used to run the event loop, as this
   * makes execution simpler.
   */
  void testRecord() {
    CharPointers argv = getArgv(
        {"--record", "--camera", App::debug_video.string(), "--interval", "1s",
         "--duration", "2s", "--folder", tmpDir.string()});
    App app(argv.size(), argv.data());
    int exitCode = app.run();
    QTest::qWait(4000);
    QCOMPARE(numberOfFilesRecursive(tmpDir), 2);
  }
};

TEST(testApp, testCameraRecord) {
  TestCamera test;
  ASSERT_EQ(QTest::qExec(&test), 0);
}

#include "test_app.moc"
