#include <boost/dll.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cstdint>
#include <filesystem>
#include <gtest/gtest.h>
#include <helpers/config.hpp>
#include <helpers/path.hpp>
#include <memory>
#include <snapshotapp.hpp>
#include <string>

using path = std::filesystem::path;
using ptree = boost::property_tree::ptree;

extern const path static_dir;

/**
 * @brief Tests the Config::parse function.
 */
TEST(testConfig, parseConfig) {
  path path = static_dir / "config_user_test.json";
  ptree config = Config::parse(path.string());

  ASSERT_EQ(config.get<bool>("gui"), true);
  ASSERT_EQ(config.get<bool>("list"), true);
  ASSERT_EQ(config.get<std::string>("camera"), "usb_cam");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}

TEST(testConfig, parseConfigInvalid) {
  path path = "/foo/bar/config.json";
  try {
    Config::parse(path.string(), true);
    FAIL();
  } catch (std::invalid_argument &e) {
    SUCCEED();
  }
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

  ptree result = Config::merge(pconfig, user);

  ASSERT_EQ(result.get<bool>("foo"), true);
  ASSERT_EQ(result.get<bool>("bar"), false);
}
