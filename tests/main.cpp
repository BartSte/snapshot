#include "helpers/path.hpp"
#include <gtest/gtest.h>

using path = std::filesystem::path;

extern const path static_dir(Path::program_location().parent_path() / "static");
extern const path config_test(static_dir / "config_test.json");
extern const path debug_video(static_dir / "sample.mp4");

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
