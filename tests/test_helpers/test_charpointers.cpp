#include <gtest/gtest.h>
#include <helpers/charpointers.hpp>

TEST(testCharPointers, testConstructor) {
  CharPointers pointers({"foo", "bar", "baz"});
  EXPECT_EQ(pointers.size(), 3);

  char **ptr = pointers.data();
  EXPECT_STREQ(ptr[0], "foo");
  EXPECT_STREQ(ptr[1], "bar");
  EXPECT_STREQ(ptr[2], "baz");
}

TEST(testCharPointers, testPushBack) {
  CharPointers pointers({"foo", "bar"});
  pointers.push_back("baz");
  EXPECT_EQ(pointers.size(), 3);
  EXPECT_STREQ(pointers.data()[2], "baz");
}

TEST(testCharPointers, testSize) {
  CharPointers pointers({"foo", "bar", "baz"});
  EXPECT_EQ(pointers.size(), 3);
  pointers.push_back("qux");
  EXPECT_EQ(pointers.size(), 4);
}
