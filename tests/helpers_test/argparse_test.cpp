#include <cxxopts.hpp>
#include <gtest/gtest.h>

#define private public

#include <helpers/argparse.hpp>

/**
 * @brief Tests the ArgParse::parse() function.
 */
TEST(argparseTest, parseTest) {
  int argc = 7;
  char *argv[7] = {const_cast<char *>("./test"),
                   const_cast<char *>("--help"),
                   const_cast<char *>("--gui"),
                   const_cast<char *>("--list"),
                   const_cast<char *>("--camera=usb_cam"),
                   const_cast<char *>("--loglevel=DEBUG"),
                   const_cast<char *>("--pattern=[foo]")};

  ArgParse parser(argc, argv);
  cxxopts::ParseResult result = parser.parse();

  EXPECT_EQ(result.count("help"), 1);
  EXPECT_EQ(result.count("gui"), 1);
  EXPECT_EQ(result.count("list"), 1);
  EXPECT_EQ(result["camera"].as<std::string>(), "usb_cam");
  EXPECT_EQ(result["loglevel"].as<std::string>(), "DEBUG");
  EXPECT_EQ(result["pattern"].as<std::string>(), "[foo]");
}