#include <helpers/argparse.hpp>

#include <boost/property_tree/ptree.hpp>
#include <cxxopts.hpp>
#include <filesystem>
#include <helpers/path.hpp>
#include <helpers/time.hpp>
#include <string>

#define DEFAULT cxxopts::value<std::string>()->default_value

using sec = std::chrono::seconds;
using path = std::filesystem::path;
using ptree = boost::property_tree::ptree;

/**
 * @brief Convert a vector of cxxopts::KeyValue to a
 * boost::property_tree::ptree.
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

  path path_config(Path::expand("${HOME}/.config/snapshot/config.json"));
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

/**
 * @brief checkLogLevel
 *
 * Checks if the loglevel is valid. If not, an exception is thrown.
 *
 * @param config The config.
 */
void checkLogLevel(const ptree &config) {
  const std::string actual = config.get<std::string>("loglevel");
  const std::vector<std::string> expected = {
      "trace", "debug", "info", "warn", "warning", "error", "critical"};

  if (std::find(expected.begin(), expected.end(), actual) == expected.end()) {
    throw std::invalid_argument("Invalid loglevel: " + actual);
  }
}

/**
 * @brief checkChrono
 *
 * Checks if the strings representing durations can be converted to
 * std::chrono::seconds. If not, an exception is thrown.
 *
 * @param config The config.
 */
void checkChrono(const ptree &config) {
  std::vector<std::string> keys = {"timeout", "duration", "interval"};
  for (const auto &key : keys) {
    const std::string str = config.get<std::string>(key);
    sec value = stringToSec(str); // throws if invalid
  }
}

/**
 * @brief check if the string representing a number in scientific notation is
 * valid. If not, an exception is thrown.
 *
 * @param config The config.
 */
void checkScientificNotation(const ptree &config) {
  const std::string str = config.get<std::string>("max-bytes");
  std::regex regex("^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$");
  if (!std::regex_match(str, regex)) {
    throw std::invalid_argument("Invalid scientific notation: " + str);
  }
}

/**
 * @brief check if the parent directory of the folders are writable such that
 * the directory can be created. If not, an exception is thrown.
 *
 * @param config
 */
void checkPaths(const ptree &config) {
  std::filesystem::path path(Path::expand(config.get<std::string>("folder")));
  std::filesystem::path parent = path.parent_path();

  if (!std::filesystem::exists(parent)) {
    throw std::invalid_argument("Parent directory does not exist: " +
                                parent.string());
  } else if (access(parent.string().c_str(), W_OK) != 0) {
    throw std::invalid_argument("Parent directory is not writable: " +
                                parent.string());
  }
}

/**
 * @brief check
 *
 * Checks if the config is valid. If not, an exception is thrown.
 *
 * @param config The config.
 */
void ArgParse::check(const ptree &config) {
  checkLogLevel(config);
  checkChrono(config);
  checkScientificNotation(config);
  checkPaths(config);
}
