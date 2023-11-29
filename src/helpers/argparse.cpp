#include <boost/filesystem.hpp>
#include <cxxopts.hpp>
#include <iostream>

#include "./helpers/argparse.hpp"

#define DEFAULT_STRING cxxopts::value<std::string>()->default_value

namespace fs = boost::filesystem;

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

  fs::path home(getHome());
  fs::path path_config(home / ".config" / "snapshot" / "config.json");
  fs::path path_save(fs::current_path() / "snapshot");

  // clang-format off
  options.add_options()
    ("h,help", "Print help")

    ("g,gui", "Start the GUI")

    ("l,list", "List available cameras")

    ("d,debug", "Enable debug mode. Plays an internal video and show the gui.")

    ("r,record", "Record")

    ("folder", "Folder to save the images to. The default is $PWD/snapshot. "
     "The folder will be created if it does not exist.",
     DEFAULT_STRING(path_save.string()))

    ("duration", "Duration of the recording in seconds. The following formats "
     "are supported: s, second, seconds, m, minute, minutes, h, hour, hours, "
     "d, day, days. For example, 10s = 10 seconds. The default is 0, meaning "
     "that the recording will continue until the user stops it.",
     DEFAULT_STRING("0"))

    ("interval", "The interval in which frames are saved. The following "
     "formats are supported: s, second, seconds, m, minute, minutes, h, hour, "
     "hours, " "d, day, days. For example, 10s = 10 seconds. The default is 0,"
     " meaning that no frames are saved.",
     DEFAULT_STRING("0"))

    ("c,camera", "Select a camera by the name shown by the list command",
     DEFAULT_STRING("default"))

    ("config", "Path to the config file",
    DEFAULT_STRING(path_config.string()))

    ("loglevel", "Set the loglevel to CRITICAL, ERROR, WARNING, INFO, or DEBUG",
    DEFAULT_STRING("warning"))

    ("pattern", "Set the log pattern (see spdlog docs for details)",
    DEFAULT_STRING("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%# @ %!] %v"));
  // clang-format on
}

/**
 * @brief Get the home directory
 *
 * Cross platform home directory expansion.
 *
 * @return The home directory
 */
std::string ArgParse::getHome() {
  std::string home = getenv("HOME");
  if (home.empty()) {
    home = getenv("USERPROFILE");
  }
  return home;
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
