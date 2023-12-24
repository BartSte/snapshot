#include <helpers/config.hpp>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <chrono>
#include <filesystem>
#include <helpers/time.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <utility>

namespace pt = boost::property_tree;
using sec = std::chrono::seconds;

/**
 * @brief parseUserDefault
 *
 * Parses two config files where first one takes precedence over the second. If
 * the first one does not exist, only the second one is used. If the second one
 * does not exist, the function will fail.
 *
 * @param path The path to a config *.json file.
 * @param path_default The path to a default config *.json file.
 *
 * @return A boost::property_tree::ptree containing the parsed config.
 */
pt::ptree config::parseUserDefault(const std::string &path_user,
                                   const std::string &path_default) {

  pt::ptree config = config::parse(path_default);
  spdlog::debug("Reading default config file from {}", path_default);

  if (std::filesystem::exists(path_user)) {
    spdlog::debug("Reading user config file from {}", path_user);
    pt::ptree config_user = config::parse(path_user);
    config::merge(config, config_user);

  } else {
    spdlog::debug("User config file does not exist.");
  }
  return config;
}

/**
 * @brief parse
 *
 * Parses a config *.json file.
 *
 * @param path The path to a config *.json file.
 * @return The parsed config as a boost::property_tree::ptree.
 */
boost::property_tree::ptree config::parse(const std::string &path) {
  pt::ptree tree;
  pt::json_parser::read_json(path, tree);
  return tree;
}

/**
 * @brief merge
 *
 * Merges two boost::property_tree::ptree objects. The second one takes
 * precedence over the first, if there are duplicate keys.
 *
 * @param config The first config.
 * @param config_user The second config.
 * @return The merged config.
 */
void config::merge(pt::ptree &config, const pt::ptree &config_user) {
  for (const auto &key_value : config_user) {
    config.put(key_value.first, key_value.second.data());
  }
}

/**
 * @brief merge
 *
 * Merges a boost::property_tree::ptree object with a cxxopts::ParseResult
 * object. The ParseResult object takes precedence over the ptree object if
 * there are duplicate keys.
 *
 * @param config The config.
 * @param args The ParseResult object.
 * @return The merged config.
 */
void config::merge(pt::ptree &config, const cxxopts::ParseResult &args) {
  for (const auto &key_value : args.arguments()) {
    config.put(key_value.key(), key_value.value());
  }
}

/**
 * @brief check
 *
 * Checks if the config is valid. If not, an exception is thrown.
 *
 * @param config The config.
 */
void config::check(const pt::ptree &config) {
  config::checkLogLevel(config);
  config::checkChrono(config);
  config::checkScientificNotation(config);
  // TODO: when the user config is not found it normally means the user did not
  // create it. However, when --config argument is used to set the path of the
  // config file, and it cannot be found, it means the user made a mistake.
  // Throw an exception in this case. Furtermore, instead of hardcoding the
  // path to the config, you can specify its path in het default config file.
}

/**
 * @brief checkLogLevel
 *
 * Checks if the loglevel is valid. If not, an exception is thrown.
 *
 * @param config The config.
 */
void config::checkLogLevel(const pt::ptree &config) {
  const std::string actual = config.get<std::string>("loglevel");
  const std::vector<std::string> expected = {
      "trace", "debug", "info", "warn", "warning", "error", "critical"};

  if (std::find(expected.begin(), expected.end(), actual) == expected.end()) {
    throw std::invalid_argument("Invalid loglevel: " + actual);
  }
}

/**
 * @brief checkChrono
 *
 * Checks if the strings representing durations can be converted to
 * std::chrono::seconds. If not, an exception is thrown.
 *
 * @param config The config.
 */
void config::checkChrono(const pt::ptree &config) {
  std::vector<std::string> keys = {"timeout", "duration", "interval"};
  for (const auto &key : keys) {
    const std::string str = config.get<std::string>(key);
    sec value = stringToSec(str); // throws if invalid
  }
}

/**
 * @brief checkBytes
 *
 * Checks if the strings representing bytes can be converted to
 * uint64_t. If not, an exception is thrown.
 *
 * @param config The config.
 */
void config::checkScientificNotation(const pt::ptree &config) {
  const std::string str = config.get<std::string>("max-bytes");
  uint64_t value = scientificToUint64(str); // throws if invalid
  if (str.find('-') != std::string::npos) {
    throw std::invalid_argument("Number cannot be negative: " + str);
  }
}

/**
 * @brief scientificToUint64
 *
 * Converts a string representing a number in scientific notation to a
 * uint64_t.
 *
 * @param str The string to convert.
 * @return The converted number.
 */
uint64_t config::scientificToUint64(const std::string &str) {
  try {
    return static_cast<uint64_t>(std::stod(str));
  } catch (...) {
    throw std::invalid_argument("Invalid number: " + str);
  }
}
