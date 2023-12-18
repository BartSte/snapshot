#include <filesystem>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <utility>

#include "./config.hpp"

namespace pt = boost::property_tree;

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
 * Merges two boost::property_tree::ptree objects. The first one takes
 * precedence over the second if there are duplicate keys.
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
