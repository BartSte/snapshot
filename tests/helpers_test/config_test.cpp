#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <gtest/gtest.h>
#include <helpers/config.hpp>
#include <memory>
#include <string>

using path = boost::filesystem::path;
using ptree = boost::property_tree::ptree;

extern const boost::filesystem::path ROOT;

/**
 * @brief Tests the parseConfig function.
 */
TEST(configTest, parseConfig) {
  path path = ROOT / "static" / "config_test.json";
  ptree config = config::parse(path.string());

  ASSERT_EQ(config.get<bool>("gui"), false);
  ASSERT_EQ(config.get<bool>("list"), false);
  ASSERT_EQ(config.get<std::string>("camera"), "default");
  ASSERT_EQ(config.get<std::string>("loglevel"), "info");
  ASSERT_EQ(config.get<std::string>("pattern"), "pattern");
}

/**
 * @brief Tests the merge function.
 */
TEST(configTest, merge) {
  // makeg ptree with 1 key value pair
  ptree user;
  ptree pconfig;

  user.put("foo", true);
  pconfig.put("foo", false);
  pconfig.put("bar", false);

  config::merge(pconfig, user);

  ASSERT_EQ(pconfig.get<bool>("foo"), true);
  ASSERT_EQ(pconfig.get<bool>("bar"), false);
}

/**
 * @brief The config_user_test should overwrite the config_test settings.
 */
TEST(configTest, parseConfigs) {
  path path_user = ROOT / "static" / "config_user_test.json";
  path path_default = ROOT / "static" / "config_test.json";

  ptree config =
      config::parseUserDefault(path_user.string(), path_default.string());

  ASSERT_EQ(config.get<bool>("gui"), true);
  ASSERT_EQ(config.get<bool>("list"), true);
  ASSERT_EQ(config.get<std::string>("camera"), "usb_cam");
  ASSERT_EQ(config.get<std::string>("loglevel"), "debug");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}

TEST(configTest, parseConfigsTestIncomplete) {
  path path_user = ROOT / "static" / "config_user_incomplete_test.json";
  path path_default = ROOT / "static" / "config_test.json";

  ptree config =
      config::parseUserDefault(path_user.string(), path_default.string());

  ASSERT_EQ(config.get<bool>("gui"), false);
  ASSERT_EQ(config.get<bool>("list"), false);
  ASSERT_EQ(config.get<std::string>("camera"), "default");
  ASSERT_EQ(config.get<std::string>("loglevel"), "info");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}
