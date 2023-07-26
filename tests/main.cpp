#include <boost/filesystem.hpp>
#include <boost/dll.hpp>
#include <gtest/gtest.h>

namespace fs = boost::filesystem;

extern const fs::path root = boost::dll::program_location().parent_path();

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
