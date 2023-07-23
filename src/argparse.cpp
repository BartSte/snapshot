#include "./argparse.hpp"
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
      description("Command line interface for taking camera snapshots.\n"),
      options(program, description) {

  // clang-format off
  options.add_options()
    ("h,help", "Print help")
    ("g,gui", "Start the GUI")
    ("loglevel", "Set the loglevel to CRITICAL, ERROR, WARNING, INFO, or DEBUG",
    cxxopts::value<std::string>()->default_value("INFO"))
    ("pattern", "Set the log pattern",
    cxxopts::value<std::string>()->default_value("[%Y-%m-%d %H:%M:%S.%e] " \
                                                 "[%l] [%s:%# @ %!] %v"));
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
