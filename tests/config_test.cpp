#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <config.hpp>
#include <gtest/gtest.h>
#include <string>

namespace fs = boost::filesystem;

extern const boost::filesystem::path root;

/**
 * @brief Tests the ArgParse::parse() function.
 */
TEST(configTestparseConfig, BasicAssertions) {
  fs::path path = root / "static" / "config_test.ini";
  boost::property_tree::ptree config = parseConfig(path.string());
  ASSERT_EQ(config.get<bool>("execution.gui"), false);
  ASSERT_EQ(config.get<bool>("execution.list"), false);
  ASSERT_EQ(config.get<std::string>("logging.loglevel"), "info");
}
