#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cxxopts.hpp>
#include <memory>
#include <string>

namespace Config {

using ptree = boost::property_tree::ptree;

ptree parse(const std::string &path, bool check = false);
ptree merge(ptree destination, const ptree &source);
} // namespace config
