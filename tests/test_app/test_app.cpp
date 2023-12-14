#include <app/app.hpp>
#include <cstdio>
#include <gtest/gtest.h>
#include <helpers/argv.hpp>
#include <helpers/cout_redirect.hpp>
#include <string>
#include <vector>

using path = std::filesystem::path;

extern const std::filesystem::path root;
const path config = root / "static" / "config_test.json";

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
Argv getArgv(std::vector<std::string> vec = {}) {
  Argv argv({"test_app", "--config", config.string()});
  for (auto &str : vec) {
    argv.append(str);
  }
  return argv;
}

TEST(testAppHelper, testGetArgv) {
  Argv argv = getArgv({"foo", "bar"});
  EXPECT_EQ(argv.getArgc(), 5);
  EXPECT_STREQ(argv.get()[0], "test_app");
  EXPECT_STREQ(argv.get()[1], "--config");
  EXPECT_STREQ(argv.get()[2], config.string().c_str());
  EXPECT_STREQ(argv.get()[3], "foo");
  EXPECT_STREQ(argv.get()[4], "bar");
}

TEST_F(TestApp, testPrintHelp) {
  Argv argv = getArgv({"--help"});
  App app(argv.getArgc(), argv.get());
  std::cout << app.run();

  EXPECT_TRUE(buffer.str().find("Usage:") != std::string::npos);
}

TEST_F(TestApp, testList) {
  Argv argv = getArgv({"--list"});
  App app(argv.getArgc(), argv.get());
  std::cout << app.run();

  EXPECT_TRUE(buffer.str().find("Index | Name | ID | Is Default |") !=
              std::string::npos);
}