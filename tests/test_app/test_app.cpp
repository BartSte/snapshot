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
using ptree = boost::property_tree::ptree;

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

TEST_F(GTestApp, testParseConfig) {
  CharPointers argv = getArgv();
  App app(argv.size(), argv.data());
  ptree settings = app.getSettings();

  // Changed by config
  ASSERT_EQ(settings.get<bool>("no-event-loop"), true);
  ASSERT_EQ(settings.get<std::string>("camera"), "non-existing-camera");
  ASSERT_EQ(settings.get<std::string>("loglevel"), "debug");
  ASSERT_EQ(settings.get<std::string>("max-bytes"), "1e9");

  // Changed by cli
  path test_config = App::static_dir / "config_test.json";
  ASSERT_EQ(settings.get<std::string>("config"), test_config.string());

  // Defaults
  path cwd = std::filesystem::current_path();
  path folder = cwd / "snapshot";
  std::string pattern = "[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%# @ %!] %v";
  ASSERT_EQ(settings.get<bool>("help"), false);
  ASSERT_EQ(settings.get<bool>("gui"), false);
  ASSERT_EQ(settings.get<bool>("list"), false);
  ASSERT_EQ(settings.get<bool>("debug"), false);
  ASSERT_EQ(settings.get<bool>("record"), false);
  ASSERT_EQ(settings.get<std::string>("folder"), folder.string());
  ASSERT_EQ(settings.get<std::string>("timeout"), "30s");
  ASSERT_EQ(settings.get<std::string>("duration"), "0s");
  ASSERT_EQ(settings.get<std::string>("interval"), "0s");
  ASSERT_EQ(settings.get<std::string>("pattern"), pattern);
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

class QTestApp : public QObject {
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
   * Normally, the --record starts the event loop. For testing, the event loop
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

  /**
   * @brief Test the --record and the --max-bytes arguments.
   *
   * Normally, the --record starts the event loop. For testing, the event loop
   * is not started. Instead, qWait is used to run the event loop, as this
   * makes execution simpler. The number of bytes is set to 1, which is so low
   * that only 1 frame can be recorded.
   */
  void testRecordMaxBytes() {
    CharPointers argv = getArgv(
        {"--record", "--camera", App::debug_video.string(), "--interval", "1s",
         "--max-bytes", "1", "--folder", tmpDir.string()});
    App app(argv.size(), argv.data());
    int exitCode = app.run();
    QTest::qWait(2000);
    QCOMPARE(numberOfFilesRecursive(tmpDir), 1);
  }
};

TEST(testApp, testCameraRecord) {
  QTestApp test;
  ASSERT_EQ(QTest::qExec(&test), 0);
}

#include "test_app.moc"
