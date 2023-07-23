#include <cxxopts.hpp>
#include <gtest/gtest.h>

#define private public

#include <argparse.h>

/**
 * @brief Tests the ArgParse::parse() function.
 */
TEST(parseTestParse, BasicAssertions) {
  int argc = 5;
  char *argv[5] = {const_cast<char *>("./test"), const_cast<char *>("--help"),
                   const_cast<char *>("--gui"),
                   const_cast<char *>("--loglevel=DEBUG"),
                   const_cast<char *>("--pattern=[foo]")};

  ArgParse parser(argc, argv);
  cxxopts::ParseResult result = parser.parse();

  EXPECT_EQ(result.count("help"), 1);
  EXPECT_EQ(result.count("gui"), 1);
  EXPECT_EQ(result["loglevel"].as<std::string>(), "DEBUG");
  EXPECT_EQ(result["pattern"].as<std::string>(), "[foo]");
}
