#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cxxopts.hpp>
#include <memory>
#include <string>

namespace pt = boost::property_tree;

namespace config {

pt::ptree parseUserDefault(const std::string &path_user,
                           const std::string &path_default);
pt::ptree parse(const std::string &path);
void merge(pt::ptree &config, const pt::ptree &config_user);
void merge(pt::ptree &config, const cxxopts::ParseResult &args);

} // namespace config
