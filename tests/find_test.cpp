#include <gtest/gtest.h>
#include <camera/find.hpp>
#include <iostream>

/**
 * @brief Tests the ArgParse::parse() function.
 */
TEST(findTest, listCameras) {
  std::string result = listCameras();
  std::cout << result << std::endl;
}

