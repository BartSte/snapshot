#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <gtest/gtest.h>
#include <helpers/config.hpp>
#include <memory>
#include <string>

namespace fs = boost::filesystem;
namespace pt = pt;

extern const boost::filesystem::path ROOT;

/**
 * @brief Tests the parseConfig function.
 */
TEST(configTest, parseConfig) {
  fs::path path = ROOT / "static" / "config_test.json";

  pt::ptree config = config::parse(path.string());

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
  pt::ptree user;
  pt::ptree pconfig;

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
  fs::path path = ROOT / "static" / "config_user_test.json";
  fs::path path_default = ROOT / "static" / "config_test.json";

  pt::ptree config =
      config::parseUserDefault(path.string(), path_default.string());

  ASSERT_EQ(config.get<bool>("gui"), true);
  ASSERT_EQ(config.get<bool>("list"), true);
  ASSERT_EQ(config.get<std::string>("camera"), "usb_cam");
  ASSERT_EQ(config.get<std::string>("loglevel"), "debug");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}

TEST(configTest, parseConfigsTestIncomplete) {
  fs::path path = ROOT / "static" / "config_user_incomplete_test.json";
  fs::path path_default = ROOT / "static" / "config_test.json";

  pt::ptree config =
      config::parseUserDefault(path.string(), path_default.string());

  ASSERT_EQ(config.get<bool>("gui"), false);
  ASSERT_EQ(config.get<bool>("list"), false);
  ASSERT_EQ(config.get<std::string>("camera"), "default");
  ASSERT_EQ(config.get<std::string>("loglevel"), "info");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}
