#include <boost/property_tree/ptree_fwd.hpp>
#include <config.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <QFile>

/**
 * @brief Tests the ArgParse::parse() function.
 */
TEST(configTestparseConfig, BasicAssertions) {
  boost::property_tree::ptree config = parseConfig("./static/config.ini");
  ASSERT_EQ(config.get<bool>("execution.gui"), false);
  ASSERT_EQ(config.get<bool>("execution.list"), false);
  ASSERT_EQ(config.get<std::string>("logging.loglevel"), "info");
}
