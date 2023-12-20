#include <QTest>
#include <cstdio>
#include <gtest/gtest.h>
#include <helpers/charpointers.hpp>
#include <helpers/cout_redirect.hpp>
#include <qobject.h>
#include <snapshotapp.hpp>
#include <string>
#include <vector>
#include <video/connect.hpp>

using path = std::filesystem::path;

extern const std::filesystem::path static_dir;
const path config = static_dir / "config_test.json";
const path debug_video = static_dir / "sample.mp4";

class TestApp : public testing::Test {
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
  std::vector<std::string> strings = {"test_app", "--config", config.string()};
  CharPointers argv(strings);
  for (auto &str : vec) {
    argv.push_back(str);
  }
  return argv;
}

TEST(testAppHelper, testGetArgv) {
  CharPointers argv = getArgv({"foo", "bar"});
  EXPECT_EQ(argv.size(), 5);
  EXPECT_STREQ(argv.data()[0], "test_app");
  EXPECT_STREQ(argv.data()[1], "--config");
  EXPECT_STREQ(argv.data()[2], config.string().c_str());
  EXPECT_STREQ(argv.data()[3], "foo");
  EXPECT_STREQ(argv.data()[4], "bar");
}

TEST_F(TestApp, testPrintHelp) {
  CharPointers argv = getArgv({"--help"});
  App app(argv.size(), argv.data());
  std::cout << app.run();

  EXPECT_TRUE(buffer.str().find("Usage:") != std::string::npos);
}

TEST_F(TestApp, testList) {
  CharPointers argv = getArgv({"--list"});
  App app(argv.size(), argv.data());
  std::cout << app.run();

  for (auto &column : {"Index", "Name", "ID", "Is Default"}) {
    EXPECT_TRUE(buffer.str().find(column) != std::string::npos);
  }
}

class TestCamera : public QObject {
  Q_OBJECT

 private slots:
  void test() {
    CharPointers argv = getArgv({"--camera", debug_video.string()});
    App app(argv.size(), argv.data());
    int exitCode = app.run();
    QTest::qWait(2000);
    app.quit();
    app.exit(exitCode);
  }
};

TEST(testApp, testCamera) {
  TestCamera test;
  ASSERT_EQ(QTest::qExec(&test), 0);
}
#include "test_app.moc"
