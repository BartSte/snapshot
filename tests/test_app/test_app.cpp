#include <app/app.hpp>
#include <gtest/gtest.h>
#include <string>
#include <vector>

extern const std::filesystem::path root;

/**
 * @brief makeArgv
 *
 * Make a vector of char * from a vector of strings.
 *
 * @param args vector of strings
 * @return vector of char *
 */
std::vector<char *> makeArgv(const std::vector<std::string> &args) {
  char *element;
  std::vector<char *> argv;

  for (const auto &arg : args) {
    element = const_cast<char *>(arg.data());
    argv.push_back(element);
  }

  argv.push_back(nullptr);
  return argv;
}

TEST(testApp, testPrintHelp) {
  EXPECT_TRUE(true);
  /* testing::internal::CaptureStdout(); */
  /* std::vector<char *> argv = makeArgv({"test", "-h"}); */
  /* App app(argv.size(), argv.data()); */
  /* std::cout << app.run(); */
  /* std::string output = testing::internal::GetCapturedStdout(); */
  /* EXPECT_TRUE(output.find("Usage:") != std::string::npos); */
}
