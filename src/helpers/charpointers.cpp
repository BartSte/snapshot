#include <algorithm>
#include <vector>

#include "./charpointers.hpp"

/**
 * @brief Construct a vector of char pointers from a vector of strings.
 *
 * @param strings A vector of strings.
 */
CharPointers::CharPointers(const std::vector<std::string> &strings) {
  pointers.reserve(strings.size());
  auto func = [this](const std::string &s) { push_back(s); };
  std::for_each(strings.begin(), strings.end(), func);
}

/**
 * @brief Destroy the char * within the vector.
 */
CharPointers::~CharPointers() {
  auto func = [](char *p) { delete[] p; };
  std::for_each(pointers.begin(), pointers.end(), func);
}

/**
 * @brief Adapter for the data() method of the vector.
 *
 * @return
 */
char **CharPointers::data() { return pointers.data(); }

/**
 * @brief Adapter for the push_back method of the vector.
 *
 * @param s
 */
void CharPointers::push_back(const std::string &s) {
  char *p = new char[s.length() + 1];
  std::copy(s.begin(), s.end(), p);
  p[s.length()] = '\0';
  pointers.push_back(p);
}

/**
 * @brief Adapter for the size method of the vector.
 *
 * @return
 */
int CharPointers::size() { return pointers.size(); }
