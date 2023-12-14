#pragma once

#include <string>
#include <vector>

class Argv {

 private:
  int argc;
  char **argv;

 public:
  explicit Argv(std::vector<std::string> vec);
  ~Argv();
  int getArgc();
  char **get();
  void append(std::string str);
};
