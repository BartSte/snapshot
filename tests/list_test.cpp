#include <gtest/gtest.h>
#include <list.hpp>
#include <iostream>

/**
 * @brief Tests the ArgParse::parse() function.
 */
TEST(listTestlistCameras, BasicAssertions) {
  std::string result = listCameras();
  std::cout << result << std::endl;
}

