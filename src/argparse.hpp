#pragma once

#include <cxxopts.hpp>
#include <string>

class ArgParse {

 public:
  ArgParse(int argc, char *argv[]);
  cxxopts::ParseResult parse();
  std::string help();

  ~ArgParse();

 private:
  int argc;
  char **argv;
  std::string program;
  std::string description;
  cxxopts::Options options;
};
