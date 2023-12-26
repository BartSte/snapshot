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
void check(const ptree &config);
void checkLogLevel(const ptree &config);
void checkChrono(const ptree &config);
void checkScientificNotation(const ptree &config);
uint64_t scientificToUint64(const std::string &str);

} // namespace config
