#include <cxxopts.hpp>
#include <gtest/gtest.h>

#define private public

#include <argparse.h>

TEST(parseTestHelp, BasicAssertions) {
  int argc = 2;
  char *argv[2] = {const_cast<char *>("./argparse_test"),
                   const_cast<char *>("--help")};

  ArgParse parser(argc, argv);
  cxxopts::ParseResult result = parser.parse();

  EXPECT_EQ(result.count("help"), 1);
}
