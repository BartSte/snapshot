#include <gtest/gtest.h>
#include <helpers/path.hpp>
#include <string>

const std::string home(getenv("HOME"));

TEST(testPath, testExpand) {
  std::string str = Path::expand("${HOME}/${HOME}/foo/bar");
  EXPECT_EQ(str, home + home + "/foo/bar");
}

TEST(testPath, testExpandNoBraces) {
  std::string str = Path::expand("$HOME/$HOME/foo/bar");
  EXPECT_EQ(str, home + home + "/foo/bar");
}

TEST(testPath, testExpandTilde) {
  std::string str = Path::expand("~/foo/~/bar");
  EXPECT_EQ(str, home + "/foo/~/bar");
  EXPECT_EQ(Path::expand("~"), home);
}

TEST(testPath, testExpandCombined) {
  std::string str = Path::expand("~/${HOME}/$HOME/~/foo/bar");
  EXPECT_EQ(str, home + home + home + "/~/foo/bar");

  // ~xxx is not expanded, ${HOME}xxx is expanded as ${HOME} with xxx appended.
  // $HOMExxx is expanded as an empty string.
  // Any // are replaced with /.
  std::string str2 = Path::expand("~xxx/${HOME}xxx/$HOMExxx/foo/bar");
  EXPECT_EQ(str2, "~xxx" + home + "xxx/foo/bar");
}

TEST(testPath, testExpandFilesystemPath) {
  std::filesystem::path path("~/${HOME}/$HOME/~/foo/bar");
  path = Path::expand(path);
  EXPECT_EQ(path.string(), home + home + home + "/~/foo/bar");
}
