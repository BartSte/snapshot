#include <filesystem>
#include <gtest/gtest.h>
#include <helpers/path.hpp>
#include <qapplication.h>

extern const std::filesystem::path root = program_location();

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  QApplication app(argc, argv);
  return RUN_ALL_TESTS();
}
