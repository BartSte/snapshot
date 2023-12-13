#include <iostream>
#include <string>
#include <vector>

#include "./helpers.hpp"

/**
 * @brief makeArgv
 *
 * Make a vector of char * from a vector of strings.
 *
 * @param args vector of strings
 * @return vector of char *
 */
std::vector<char *> makeArgv(const std::vector<std::string> &args) {
  char *element;
  std::vector<char *> argv;

  for (const auto &arg : args) {
    element = const_cast<char *>(arg.data());
    argv.push_back(element);
  }

  argv.push_back(nullptr);
  return argv;
}

/**
 * @class cout_redirect
 * @brief
 *
 */
CoutRedirect::CoutRedirect(std::streambuf *new_buffer)
    : old(std::cout.rdbuf(new_buffer)) {}

CoutRedirect::~CoutRedirect() { std::cout.rdbuf(old); }
