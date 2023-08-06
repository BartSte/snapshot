#include <boost/property_tree/ptree_fwd.hpp>
#include <memory>
#include <utility>
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
  std::unique_ptr<pt::ptree> pconfig = std::make_unique<pt::ptree>(config);
  SPDLOG_DEBUG("Reading default config file from {}", path_default);

  if (fs::exists(path_user)) {
    SPDLOG_DEBUG("Reading user config file from {}", path_user);
    pt::ptree config_user = config::parse(path_user);
    pconfig = config::merge(std::move(pconfig), config_user);

  } else {
    SPDLOG_INFO("User config file does not exist.");
  }
  return *pconfig;
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
std::unique_ptr<pt::ptree> config::merge(std::unique_ptr<pt::ptree> config,
                                         const pt::ptree &config_user) {
  for (const auto &key_value : config_user) {
    config->put(key_value.first, key_value.second.data());
  }
  return std::move(config);
}

/**
 * @brief merge
 *
 * Merges a boost::property_tree::ptree object with a
 * cxxopts::ParseResult object. The ParseResult object takes precedence over the
 * ptree object if there are duplicate keys.
 *
 * @param config The config.
 * @param args The ParseResult object.
 * @return The merged config.
 */
std::unique_ptr<pt::ptree> config::merge(std::unique_ptr<pt::ptree> config,
                                         const cxxopts::ParseResult &args) {
  for (const auto &key_value : args.arguments()) {
    config->put(key_value.key(), key_value.value());
  }
  return std::move(config);
}
