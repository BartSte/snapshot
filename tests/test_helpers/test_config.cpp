#include <boost/dll.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <filesystem>
#include <gtest/gtest.h>
#include <helpers/config.hpp>
#include <helpers/path.hpp>
#include <memory>
#include <string>

using path = std::filesystem::path;
using ptree = boost::property_tree::ptree;

extern const path static_dir;

/**
 * @brief Tests the parseConfig function.
 */
TEST(testConfig, parseConfig) {
  path path = static_dir / "config_default_test.json";
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
TEST(testConfig, merge) {
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
 * @brief The config_user_test should overwrite the config_default_test
 * settings.
 */
TEST(testConfig, parseConfigs) {
  path path_user = static_dir / "config_user_test.json";
  path path_default = static_dir / "config_default_test.json";

  ptree config =
      config::parseUserDefault(path_user.string(), path_default.string());

  ASSERT_EQ(config.get<bool>("gui"), true);
  ASSERT_EQ(config.get<bool>("list"), true);
  ASSERT_EQ(config.get<std::string>("camera"), "usb_cam");
  ASSERT_EQ(config.get<std::string>("loglevel"), "debug");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}

TEST(testConfig, parseConfigsTestIncomplete) {
  path path_user = static_dir / "config_user_incomplete_test.json";
  path path_default = static_dir / "config_default_test.json";

  ptree config =
      config::parseUserDefault(path_user.string(), path_default.string());

  ASSERT_EQ(config.get<bool>("gui"), false);
  ASSERT_EQ(config.get<bool>("list"), false);
  ASSERT_EQ(config.get<std::string>("camera"), "default");
  ASSERT_EQ(config.get<std::string>("loglevel"), "info");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}
