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

using sec = std::chrono::seconds;
using ptree = boost::property_tree::ptree;

/**
 * @brief parses the config file at `path`.
 *
 * An empty ptree is returned if the file does not exist.
 *
 * @param path The path to a config *.json file.
 *
 * @return The parsed config as a boost::property_tree::ptree.
 */
boost::property_tree::ptree Config::parse(const std::string &path, bool check) {
  ptree tree;
  if (std::filesystem::exists(path)) {
    spdlog::debug("Reading config file from {}", path);
    boost::property_tree::json_parser::read_json(path, tree);
  } else if (check) {
    throw std::invalid_argument("Config file at " + path + " does not exist.");
  }
  return tree;
}

/**
 * @brief merges two boost::property_tree::ptree objects.
 *
 * Merges two boost::property_tree::ptree objects. The second one takes
 * precedence over the first, if there are duplicate keys.
 *
 * @param config The first config.
 * @param config_user The second config.
 *
 * @return The merged config.
 */
ptree Config::merge(ptree destination, const ptree &source) {
  for (const auto &key_value : source) {
    destination.put(key_value.first, key_value.second.data());
  }
  return destination;
}

/**
 * @brief check
 *
 * Checks if the config is valid. If not, an exception is thrown.
 *
 * @param config The config.
 */
void Config::check(const ptree &config) {
  Config::checkLogLevel(config);
  Config::checkChrono(config);
  Config::checkScientificNotation(config);
}

/**
 * @brief checkLogLevel
 *
 * Checks if the loglevel is valid. If not, an exception is thrown.
 *
 * @param config The config.
 */
void Config::checkLogLevel(const ptree &config) {
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
void Config::checkChrono(const ptree &config) {
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
void Config::checkScientificNotation(const ptree &config) {
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
uint64_t Config::scientificToUint64(const std::string &str) {
  try {
    return static_cast<uint64_t>(std::stod(str));
  } catch (...) {
    throw std::invalid_argument("Invalid number: " + str);
  }
}
