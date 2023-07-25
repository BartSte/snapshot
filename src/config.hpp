#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>

boost::property_tree::ptree parseConfig(const std::string &filename);
