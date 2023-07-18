#include "./argparse.h"
#include <cxxopts.hpp>
#include <iostream>

/**
 * @brief Construct a new Arg Parse:: Arg Parse object
 *
 * @param argc The number of arguments
 * @param argv The arguments
 */
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

/**
 * @brief Parse the arguments
 *
 * @return a cxxopts::ParseResult object
 */
cxxopts::ParseResult ArgParse::parse() { return options.parse(argc, argv); }

/**
 * @brief Get the help string
 *
 * @return The help string
 */
std::string ArgParse::help() { return options.help(); }

ArgParse::~ArgParse() {}
