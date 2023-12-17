#pragma once

#include <string>
#include <vector>

class CharPointers {
 private:
  std::vector<char *> pointers;

 public:
  explicit CharPointers(const std::vector<std::string> &strings);
  ~CharPointers();
  char **data();
  void push_back(const std::string &s);
  int size();
};
