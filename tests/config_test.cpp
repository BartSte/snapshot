#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <helpers/config.hpp>
#include <gtest/gtest.h>
#include <string>

namespace fs = boost::filesystem;

extern const boost::filesystem::path root;

/**
 * @brief Tests the parseConfig function.
 */
TEST(parseConfigTest, BasicAssertions) {
  fs::path path = root / "static" / "config_test.json";

  boost::property_tree::ptree config = parseConfig(path.string());

  ASSERT_EQ(config.get<bool>("gui"), false);
  ASSERT_EQ(config.get<bool>("list"), false);
  ASSERT_EQ(config.get<std::string>("loglevel"), "info");
}

/**
 * @brief Tests the mergeConfigs function.
 */ 
TEST(mergeConfigsTest, BasicAssertions) {
  // makeg ptree with 1 key value pair
  pt::ptree user;
  pt::ptree default_;

  user.put("foo", true);
  default_.put("foo", false);
  default_.put("bar", false);

  boost::property_tree::ptree merged = mergeConfigs(user, default_);

  ASSERT_EQ(merged.get<bool>("foo"), true);
  ASSERT_EQ(merged.get<bool>("bar"), false);
}

/**
 * @brief The config_user_test should overwrite the config_test settings.
 */
TEST(parseConfigsTest, BasicAssertions) {
  fs::path path = root / "static" / "config_user_test.json";
  fs::path path_default = root / "static" / "config_test.json";

  boost::property_tree::ptree config = parseConfigs(path.string(),
                                                    path_default.string());

  ASSERT_EQ(config.get<bool>("gui"), true);
  ASSERT_EQ(config.get<bool>("list"), true);
  ASSERT_EQ(config.get<std::string>("loglevel"), "debug");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}

TEST(parseConfigsTestIncomplete, BasicAssertions){
  fs::path path = root / "static" / "config_user_incomplete_test.json";
  fs::path path_default = root / "static" / "config_test.json";

  boost::property_tree::ptree config = parseConfigs(path.string(),
                                                    path_default.string());

  ASSERT_EQ(config.get<bool>("gui"), false);
  ASSERT_EQ(config.get<bool>("list"), false);
  ASSERT_EQ(config.get<std::string>("loglevel"), "info");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}
