#include <string>
#include <vector>

#include "helpers/argv.hpp"

/**
 * @brief Construct.
 *
 * Converst a vector of strings to a char **. Its size is argc + 1, where the
 * last element is a nullptr.
 *
 * @param vec Vector of strings.
 */
Argv::Argv(std::vector<std::string> vec) : argc(vec.size()) {
  argv = new char *[argc + 1];
  for (int i = 0; i < argc; i++) {
    argv[i] = new char[vec[i].size() + 1];
    snprintf(argv[i], vec[i].size() + 1, "%s", vec[i].c_str());
  }
  argv[argc] = nullptr;
}

/**
 * @brief Destroy.
 *
 * Frees the memory that corresponds to the elements of the array. Then, frees
 * the array itself.
 */
Argv::~Argv() {
  for (int i = 0; i < argc; i++) {
    delete[] argv[i];
  }
  delete[] argv;
}

/**
 * @brief Append a string to the array.
 *
 * @param str String to append.
 * @return char** The new array.
 */
void Argv::append(std::string str) {
  ++argc;

  char **new_argv = new char *[argc + 1];
  for (int i = 0; i < argc - 1; i++) {
    new_argv[i] = argv[i];
  }
  new_argv[argc - 1] = new char[str.size() + 1];
  snprintf(new_argv[argc - 1], str.size() + 1, "%s", str.c_str());
  new_argv[argc] = nullptr;

  delete[] argv;
  argv = new_argv;
}

int Argv::getArgc() { return argc; }

char **Argv::get() { return argv; }
