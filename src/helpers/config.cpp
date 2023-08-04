#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <spdlog/spdlog.h>
#include <string>

#include "./helpers/config.hpp"

namespace pt = boost::property_tree;
namespace fs = boost::filesystem;

/**
 * @brief parseConfigs
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
pt::ptree parseConfigs(const std::string &path_user,
                       const std::string &path_default) {

  pt::ptree config = parseConfig(path_default);
  SPDLOG_DEBUG("Reading default config file from {}", path_default);

  if (fs::exists(path_user)) {

    SPDLOG_DEBUG("Reading user config file from {}", path_user);
    pt::ptree config_user = parseConfig(path_user);
    pt::ptree config_merged = mergeConfigs(config_user, config);

    return config_merged;

  } else {

    SPDLOG_INFO("User config file does not exist.");
    return config;
  }
}

/**
 * @brief parseConfig
 *
 * Parses a config *.json file.
 *
 * @param path The path to a config *.json file.
 * @return The parsed config as a boost::property_tree::ptree.
 */
boost::property_tree::ptree parseConfig(const std::string &path) {
  pt::ptree tree;
  pt::json_parser::read_json(path, tree);
  return tree;
}

/**
 * @brief mergeConfigs
 *
 * Merges two boost::property_tree::ptree objects. The first one takes
 * precedence over the second if there are duplicate keys.
 *
 * @param config The first config.
 * @param config_user The second config.
 * @return The merged config.
 */
pt::ptree mergeConfigs(const pt::ptree &config_user, const pt::ptree &config ) {
  pt::ptree config_merged = config;
  for (const auto &kv : config_user) {
    config_merged.put(kv.first, kv.second.data());
  }
  return config_merged;
}
