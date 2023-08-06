#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cxxopts.hpp>
#include <memory>
#include <string>

namespace pt = boost::property_tree;

namespace config {

// TODO(barts) make this module use smart pointers
// TODO (barts) fix seg fault
pt::ptree parseUserDefault(const std::string &path_user,
                           const std::string &path_default);
pt::ptree parse(const std::string &path);
std::unique_ptr<pt::ptree> merge(std::unique_ptr<pt::ptree> config,
                                 const pt::ptree &config_user);
std::unique_ptr<pt::ptree> merge(std::unique_ptr<pt::ptree> config,
                                 const cxxopts::ParseResult &args);

} // namespace config
