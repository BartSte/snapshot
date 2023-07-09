#include <cxxopts.hpp>
#include <string>

class ArgParse {

 public:
  cxxopts::Options options;

  ArgParse(int argc, char *argv[]);
  void parse();

  ~ArgParse();

 private:
  int argc;
  char **argv;
  std::string title;
};
