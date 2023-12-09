#include <boost/dll.hpp>
#include <filesystem>
#include <gtest/gtest.h>
#include <helpers/path.hpp>
#include <qcoreapplication.h>

extern const std::filesystem::path root = program_location();

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  QCoreApplication app(argc, argv);
  return RUN_ALL_TESTS();
}
