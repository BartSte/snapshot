#include <filesystem>
#include <gtest/gtest.h>
#include <helpers/path.hpp>
#include <qapplication.h>

extern const std::filesystem::path root = program_location();
extern const std::filesystem::path static_dir = root / ".." / "static";

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
