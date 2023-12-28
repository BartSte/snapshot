#include <helpers/config.hpp>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <chrono>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <string>

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
