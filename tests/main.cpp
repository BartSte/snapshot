#include <boost/dll.hpp>
#include <filesystem>
#include <gtest/gtest.h>
#include <helpers/path.hpp>

extern const std::filesystem::path ROOT = program_location();

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
