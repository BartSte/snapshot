#include <helpers/argparse.hpp>

#include <boost/property_tree/ptree.hpp>
#include <cxxopts.hpp>
#include <filesystem>
#include <string>

#define DEFAULT cxxopts::value<std::string>()->default_value

using path = std::filesystem::path;
using ptree = boost::property_tree::ptree;

/**
 * @brief Convert a vector of cxxopts::KeyValue to a boost::property_tree::ptree.
 *
 * Usefull when you do not want to use cxxopts ist KeyValues type.
 *
 * @param args a vector of cxxopts::KeyValue
 * @return a ptree
 */
ptree ArgParse::asPtree(const std::vector<cxxopts::KeyValue> &args) {
  ptree tree;
  for (auto &arg : args) {
    tree.put(arg.key(), arg.value());
  }
  return tree;
}

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

  path home(getHome());
  path path_config(home / ".config" / "snapshot" / "config.json");
  path path_save(std::filesystem::current_path() / "snapshot");

  // clang-format off
  options.add_options()
    ("h,help", "Print help")

    ("g,gui", "Start the GUI")

    ("l,list", "List available cameras")

    ("d,debug", "Enable debug mode. Plays an internal video and show the gui.")

    ("r,record", "Record")

    ("n,no-event-loop", "Used for testing. Does not start the event loop.")

    ("c,camera", "Select a camera by the name shown by the list command",
     DEFAULT("default"))

    ("folder", "Folder to save the images to. The default is `./snapshot` in "
     "the current working directory. The folder will be created if it does not"
     " exist.",
     DEFAULT(path_save.string()))

    ("timeout", "Amount of time to wait after stop trying to connect "
     "to a camera. The following formats are supported: s, second, seconds, m, "
     "minute, minutes, h, hour, hours, " "d, day, days. For example, 10s = 10 "
     "seconds. If no unit is supplied, it is iterpreted as seconds. The "
     "default is 30s.",
      DEFAULT("30s"))

    ("duration", "Duration of the recording in seconds. The following formats "
     "are supported: s, second, seconds, m, minute, minutes, h, hour, hours, "
     "d, day, days. For example, 10s = 10 seconds. If no unit is supplied, it "
     "is iterpreted as seconds. The default is 0, meaning that the recording "
     "will continue until the user stops it.",
     DEFAULT("0s"))

    ("interval", "The interval in which frames are saved. The following "
     "formats are supported: s, second, seconds, m, minute, minutes, h, hour, "
     "hours, " "d, day, days. For example, 10s = 10 seconds. If no unit is "
     "supplied, it is iterpreted as seconds.The default is 0, meaning that no "
     "frames are saved.",
     DEFAULT("0s"))

    ("max-bytes", "The maximal number of bytes that may be saved within 1 "
     "session. The maximal number of bytes is 2^64 - 1. The default is "
     "10e9 (10 GB), which is large but will avoid your disk filling "
     "up when you forget to stop the recording. Scientific notation is "
     "supported.",
     DEFAULT("10e9"))

    ("config", "Path to the config file",
    DEFAULT(path_config.string()))

    ("loglevel", "Set the loglevel to CRITICAL, ERROR, WARNING, INFO, or DEBUG",
    DEFAULT("warning"))

    ("pattern", "Set the log pattern (see spdlog docs for details)",
    DEFAULT("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%# @ %!] %v"));
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
