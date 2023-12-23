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

/**
 * @brief Tests the parseConfig function.
 */
TEST(testConfig, parseConfig) {
  path path = App::static_dir / "config_default_test.json";
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
  path path_user = App::static_dir / "config_user_test.json";
  path path_default = App::static_dir / "config_default_test.json";

  ptree config =
      config::parseUserDefault(path_user.string(), path_default.string());

  ASSERT_EQ(config.get<bool>("gui"), true);
  ASSERT_EQ(config.get<bool>("list"), true);
  ASSERT_EQ(config.get<std::string>("camera"), "usb_cam");
  ASSERT_EQ(config.get<std::string>("loglevel"), "debug");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}

TEST(testConfig, parseConfigsTestIncomplete) {
  path path_user = App::static_dir / "config_user_incomplete_test.json";
  path path_default = App::static_dir / "config_default_test.json";

  ptree config =
      config::parseUserDefault(path_user.string(), path_default.string());

  ASSERT_EQ(config.get<bool>("gui"), false);
  ASSERT_EQ(config.get<bool>("list"), false);
  ASSERT_EQ(config.get<std::string>("camera"), "default");
  ASSERT_EQ(config.get<std::string>("loglevel"), "info");
  ASSERT_EQ(config.get<std::string>("pattern"), "foo");
}

/*
 * @brief tryCatchSucceed
 *
 * Takes a function as argument and executes it in a try catch block. If an
 * std::invalid_argument is thrown, the test succeeds, otherwise it fails.
 * @param function The function to execute.
 */
void tryCatchSucceed(std::function<void()> function) {
  try {
    function();
    FAIL();
  } catch (std::invalid_argument &e) {
    SUCCEED();
  }
}

/**
 * @brief tryCatchFail
 *
 * Takes a function as argument and executes it in a try catch block. If an
 * std::invalid_argument is thrown, the test fails, otherwise it succeeds.
 *
 * @param function The function to execute.
 */
void tryCatchFail(std::function<void()> function) {
  try {
    function();
    SUCCEED();
  } catch (std::invalid_argument &e) {
    FAIL();
  }
}

TEST(testConfig, testCheckLogLevel) {
  ptree config;
  auto func = [&config]() { config::checkLogLevel(config); };

  config.put("loglevel", "foo");
  tryCatchSucceed(func);

  config.put("loglevel", "warning");
  tryCatchFail(func);
}

TEST(testConfig, testCheckChrono) {
  ptree config;
  auto func = [&config]() { config::checkChrono(config); };

  config.put("timeout", "foo");
  config.put("interval", "foo");
  config.put("duration", "foo");
  tryCatchSucceed(func);

  config.put("timeout", "1");
  config.put("interval", "1");
  config.put("duration", "1");
  tryCatchFail(func);
}

TEST(testConfig, testCheckScientificNotation) {
  ptree config;
  auto func = [&config]() { config::checkScientificNotation(config); };

  config.put("max-bytes", "foo");
  tryCatchSucceed(func);
  config.put("max-bytes", "-1e10");
  tryCatchSucceed(func);

  config.put("max-bytes", "1e10");
  tryCatchFail(func);
}

TEST(testConfig, testScientificNotation) {
  tryCatchSucceed([]() { config::scientificToUint64("foo"); });

  try {
    uint64_t value = config::scientificToUint64("1e10");
    ASSERT_EQ(value, 10000000000);

    value = config::scientificToUint64("1e-10");
    ASSERT_EQ(value, 0);

    value = config::scientificToUint64("-1e0");
    ASSERT_EQ(value, UINT64_MAX);
  } catch (std::invalid_argument &e) {
    FAIL();
  }
}
