#pragma once

#include <string>
#include <vector>

class Argv {

 private:
  const int argc;
  char **argv;

 public:
  explicit Argv(std::vector<std::string> vec);
  ~Argv();
  int size();
  char **get();
};
