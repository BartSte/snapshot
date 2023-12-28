#pragma once

#include <cxxopts.hpp>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <vector>

class ArgParse {

  using ptree = boost::property_tree::ptree;

 public:
  static ptree asPtree(const std::vector<cxxopts::KeyValue> &args);
  static void check(const ptree &config);

  ArgParse(int argc, char *argv[]);
  cxxopts::ParseResult parse();
  std::string help();

 private:
  int argc;
  char **argv;
  std::string program;
  std::string description;
  cxxopts::Options options;
};
