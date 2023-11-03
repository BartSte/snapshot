#include <boost/filesystem.hpp>
#include <boost/dll.hpp>
#include <gtest/gtest.h>

using path = boost::filesystem::path;

extern const path ROOT = boost::dll::program_location().parent_path();

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
