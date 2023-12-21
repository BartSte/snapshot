#include <filesystem>
#include <gtest/gtest.h>
#include <helpers/path.hpp>
#include <qapplication.h>

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
