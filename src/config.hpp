#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>

namespace pt = boost::property_tree;

pt::ptree parseConfigs(const std::string &path_user,
                       const std::string &path_default);
pt::ptree parseConfig(const std::string &path);
pt::ptree mergeConfigs(const pt::ptree &config_user, const pt::ptree &config);
