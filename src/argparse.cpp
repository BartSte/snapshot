#include "./argparse.h"
#include <cxxopts.hpp>
#include <iostream>

ArgParse::ArgParse(int argc, char *argv[])
    : argc(argc),
      argv(argv),
      program(argv[0]),
      description(
          "Command line interface for taking snapshots with a camera.\n"),
      options(program, description) {
  // clang-format off
  options.add_options()
    ("h,help", "Print help")
    ("g,gui", "Start the GUI");
  // clang-format on
}

cxxopts::ParseResult ArgParse::parse() { return options.parse(argc, argv); }

std::string ArgParse::help() { return options.help(); }

ArgParse::~ArgParse() {}
