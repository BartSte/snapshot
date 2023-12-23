#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cxxopts.hpp>
#include <memory>
#include <string>

namespace config {

using ptree = boost::property_tree::ptree;

ptree parseUserDefault(const std::string &path_user,
                       const std::string &path_default);
ptree parse(const std::string &path);
void merge(ptree &config, const ptree &config_user);
void merge(ptree &config, const cxxopts::ParseResult &args);
void check(const ptree &config);
void checkLogLevel(const ptree &config);
void checkChrono(const ptree &config);
void checkScientificNotation(const ptree &config);
uint64_t scientificToUint64(const std::string &str);

} // namespace config
