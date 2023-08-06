#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <gtest/gtest.h>
#include <helpers/config.hpp>
#include <memory>
#include <string>

namespace fs = boost::filesystem;

extern const boost::filesystem::path root;

/**
 * @brief Tests the parseConfig function.
 */
TEST(parseConfigTest, BasicAssertions) {
  fs::path path = root / "static" / "config_test.json";

  boost::property_tree::ptree config = config::parse(path.string());

  ASSERT_EQ(config.get<bool>("gui"), false);
  ASSERT_EQ(config.get<bool>("list"), false);
  ASSERT_EQ(config.get<std::string>("loglevel"), "info");
}

/**
 * @brief Tests the merge function.
 */
TEST(mergeTest, BasicAssertions) {
  // makeg ptree with 1 key value pair
  pt::ptree user;
  std::unique_ptr<pt::ptree> pconfig;

  user.put("foo", true);
  pconfig->put("foo", false);
  pconfig->put("bar", false);

  pconfig = config::merge(std::move(pconfig), user);

  ASSERT_EQ(pconfig->get<bool>("foo"), true);
  ASSERT_EQ(pconfig->get<bool>("bar"), false);
}

/**
 * @brief The config_user_test should overwrite the config_test settings.
 */
TEST(parseConfigsTest, BasicAssertions) {
  fs::path path = root / "static" / "config_user_test.json";
  fs::path path_default = root / "static" / "config_test.json";

  boost::property_tree::ptree config =
      config::parseUserDefault(path.string(), path_default.string());

  ASSERT_EQ(config.get<bool>("gui"), true);
  ASSERT_EQ(config.get<bool>("list"), true);
  ASSERT_EQ(config.get<std::string>("loglevel"), "debug");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}

TEST(parseConfigsTestIncomplete, BasicAssertions) {
  fs::path path = root / "static" / "config_user_incomplete_test.json";
  fs::path path_default = root / "static" / "config_test.json";

  boost::property_tree::ptree config =
      config::parseUserDefault(path.string(), path_default.string());

  ASSERT_EQ(config.get<bool>("gui"), false);
  ASSERT_EQ(config.get<bool>("list"), false);
  ASSERT_EQ(config.get<std::string>("loglevel"), "info");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}
