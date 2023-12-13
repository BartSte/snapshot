#include <app/app.hpp>
#include <cstdio>
#include <gtest/gtest.h>
#include <helpers/argv.hpp>
#include <helpers/cout_redirect.hpp>
#include <string>
#include <vector>

extern const std::filesystem::path root;

TEST(testApp, testPrintHelp) {
  std::stringstream buffer;
  CoutRedirect redirect(buffer.rdbuf());

  Argv argv({"test_app", "--help"});
  App app(argv.size(), argv.get());
  std::cout << app.run();

  EXPECT_TRUE(buffer.str().find("Usage:") != std::string::npos);
}
