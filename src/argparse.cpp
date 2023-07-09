#include "./argparse.h"
#include <cxxopts.hpp>
#include <iostream>

ArgParse::ArgParse(int argc, char *argv[])
    : argc(argc),
      argv(argv),
      title("example - command line options"),
      options(argv[0], "") {

  options.positional_help("[optional args]").show_positional_help();

  // clang-format off
  options
    .add_options()
    ("h,help", "Print help")
    ("positional", "Positional arguments: these are the arguments that are "
     "entered without an option", cxxopts::value<std::vector<std::string>>());
  // clang-format on
}

void ArgParse::parse() {
  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help({"HELP!"}) << std::endl;
    exit(0);
  }

  if (result.count("positional")) {
    std::cout << "Positional arguments: " << std::endl;
    auto &v = result["positional"].as<std::vector<std::string>>();
    for (const auto &s : v) {
      std::cout << s << std::endl;
    }
  }
}

ArgParse::~ArgParse() {}
