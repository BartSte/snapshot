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

Argv getArgv(std::vector<std::string> vec = {}) {
  Argv argv({"test_app", "--config", config.string()});
  for (auto &str : vec) {
    argv.append(str);
  }
  return argv;
}

TEST(testApp, testGetArgv) {
  Argv argv = getArgv({"foo", "bar"});
  EXPECT_EQ(argv.getArgc(), 5);
  EXPECT_STREQ(argv.get()[0], "test_app");
  EXPECT_STREQ(argv.get()[1], "--config");
  EXPECT_STREQ(argv.get()[2], config.string().c_str());
  EXPECT_STREQ(argv.get()[3], "foo");
  EXPECT_STREQ(argv.get()[4], "bar");
}

TEST(testApp, testPrintHelp) {
  std::stringstream buffer;
  CoutRedirect redirect(buffer.rdbuf());

  Argv argv = getArgv({"--help"});
  App app(argv.getArgc(), argv.get());
  std::cout << app.run();

  EXPECT_TRUE(buffer.str().find("Usage:") != std::string::npos);
}
