#include <cxxopts.hpp>
#include <gtest/gtest.h>
#include <helpers/argparse.hpp>
#include <helpers/charpointers.hpp>
#include <helpers/path.hpp>
#include <spdlog/spdlog.h>

using ptree = boost::property_tree::ptree;

/**
 * @brief Tests the ArgParse::parse() function.
 */
TEST(testArgparse, testParse) {
  CharPointers args({"./test", "--help", "--gui", "--list", "--camera=usb_cam",
                     "--loglevel=DEBUG", "--pattern=[foo]", "--duration=10d",
                     "--interval=1hour", "--timeout=10d"});

  ArgParse parser(args.size(), args.data());
  cxxopts::ParseResult result = parser.parse();

  EXPECT_EQ(result.count("help"), 1);
  EXPECT_EQ(result.count("gui"), 1);
  EXPECT_EQ(result.count("list"), 1);
  EXPECT_EQ(result["camera"].as<std::string>(), "usb_cam");
  EXPECT_EQ(result["loglevel"].as<std::string>(), "DEBUG");
  EXPECT_EQ(result["pattern"].as<std::string>(), "[foo]");
  EXPECT_EQ(result["duration"].as<std::string>(), "10d");
  EXPECT_EQ(result["interval"].as<std::string>(), "1hour");
  EXPECT_EQ(result["timeout"].as<std::string>(), "10d");
}

ptree getDefaults() {
  CharPointers cli({"test"});
  ArgParse parser = ArgParse(cli.size(), cli.data());
  return ArgParse::asPtree(parser.parse().defaults());
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
    spdlog::error("Exception not thrown");
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
    spdlog::error("{}", e.what());
    FAIL();
  }
}

TEST(testArgparse, testCheckLogLevel) {
  ptree config = getDefaults();
  auto func = [&config]() { ArgParse::check(config); };

  config.put("loglevel", "foo");
  tryCatchSucceed(func);

  config.put("loglevel", "warning");
  tryCatchFail(func);
}

TEST(testArgparse, testCheckChrono) {
  ptree config = getDefaults();
  auto func = [&config]() { ArgParse::check(config); };

  config.put("timeout", "foo");
  config.put("interval", "foo");
  config.put("duration", "foo");
  tryCatchSucceed(func);

  config.put("timeout", "1");
  config.put("interval", "1");
  config.put("duration", "1");
  tryCatchFail(func);
}

TEST(testArgparse, testCheckScientificNotation) {
  ptree config = getDefaults();
  auto func = [&config]() { ArgParse::check(config); };

  std::vector<std::string> invalid({"foo", "1e", "1e1a", "1a10", "e10"});
  for (const auto &str : invalid) {
    config.put("max-bytes", str);
    tryCatchSucceed(func);
  }

  std::vector<std::string> valid({"1e10", "1e+10", "1e-10", "1E10", "1E+10",
                                  "1E-10", "1.0e10", "1.0e+10", "1.0e-10",
                                  "1.0E10", "1.0E+10", "1.0E-10"});
  for (const auto &str : valid) {
    config.put("max-bytes", str);
    tryCatchFail(func);
  }
}

TEST(testArgparse, testPaths) {
  ptree config = getDefaults();
  auto func = [&config]() { ArgParse::check(config); };

  config.put("folder", "/non-existing-parent/non-existing-folder");
  tryCatchSucceed(func);

  config.put("folder", "~/non-existing-folder");
  tryCatchFail(func);
}
