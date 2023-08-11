#include <gtest/gtest.h>
#include <camera/list.hpp>
#include <iostream>

/**
 * @brief Tests the ArgParse::parse() function.
 */
TEST(listTest, listTestlistCameras) {
  std::string result = listCameras();
  std::cout << result << std::endl;
}

