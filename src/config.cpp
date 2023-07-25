#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <string>

boost::property_tree::ptree parseConfig(const std::string &filename) {
  boost::property_tree::ptree tree;
  boost::property_tree::ini_parser::read_ini(filename, tree);
  return tree;
}
