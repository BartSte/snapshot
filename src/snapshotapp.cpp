#include <snapshotapp.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <chrono>
#include <cxxopts.hpp>
#include <gui/mainwindow.hpp>
#include <helpers/config.hpp>
#include <helpers/path.hpp>
#include <helpers/time.hpp>
#include <iostream>
#include <memory>
#include <qapplication.h>
#include <signal.h>
#include <spdlog/spdlog.h>
#include <string>
#include <video/find.hpp>
#include <video/record.hpp>

using ptree = boost::property_tree::ptree;
using path = std::filesystem::path;
using sec = std::chrono::seconds;
using ms = std::chrono::milliseconds;

const path App::root = Path::program_location();
const path App::static_dir = App::root / ".." / "static";
const path App::debug_video = App::static_dir / "sample.mp4";

/**
 * @brief Constructor
 *
 * Parses the cli arguments and merges them with the config file. The config is
 * used to initialize the logger, and also further in the program.
 *
 * @param argc
 * @param argv
 */
App::App(int argc, char *argv[])
    : QApplication(argc, argv), parser(argc, argv) {
  signal(SIGINT, sigintHandler);

  cxxopts::ParseResult args = parser.parse();

  std::string path_config = args["config"].as<std::string>();
  ptree defaults = ArgParse::asPtree(args.defaults());
  ptree cli = ArgParse::asPtree(args.arguments());
  settings = parseConfig(path_config, defaults, cli);

  setUpLogger(settings.get<std::string>("loglevel"),
              settings.get<std::string>("pattern"));
}

/**
 * @brief sigintHandler
 *
 * Handle the SIGINT signal.
 *
 * @param signal An integer representing the signal.
 */
void App::sigintHandler(int signal) {
  if (signal == SIGINT) {
    spdlog::info("SIGINT received, exiting...");
    auto app = App::instance();
    if (app == nullptr) {
      spdlog::warn("QApplication is a nullptr.");
    } else {
      App::instance()->quit();
    }
  }
}

/**
 * @brief setUpLogger
 *
 * Set the loglevel and the pattern of the logger.
 *
 * @param level The loglevel. Options are: trace, debug, info, warn, error,
 * critical.
 * @param pattern The pattern of the logger. See spdlog documentation for
 * details.
 */
void App::setUpLogger(std::string level, std::string pattern) {
  std::transform(level.begin(), level.end(), level.begin(), ::tolower);
  spdlog::set_level(spdlog::level::from_str(level));
  spdlog::set_pattern(pattern);
  spdlog::debug("Loglevel: {}", level);
  spdlog::debug("Pattern: {}", pattern);
}

/**
 * @brief Returns the settings for the application.
 *
 * Parses the user config file from `path_config` and merges it with the
 * defaults. The result is merged with the cli arguments. If the config is
 * invalid, an exception is thrown.
 *
 * The existence of the user config file is only checked if it differs from the
 * default path.
 *
 * @param path_config The path to the user config file.
 * @param defaults The defaults.
 *
 * @return The settings.
 */
ptree App::parseConfig(const std::string &path_config, const ptree &defaults,
                       const ptree &cli) {
  bool customConfigPath(cli.count("config"));
  ptree config_user = Config::parse(path_config, customConfigPath);

  ptree result = Config::merge(defaults, config_user);
  result = Config::merge(result, cli);

  Config::check(result); // throws if invalid

  return result;
}

/**
 * @brief exec
 *
 * Executes the program.
 *
 * @return exit code
 */
int App::run() {
  if (printHelp()) {
    return 0;
  }
  debug();
  list();
  connect();
  show();
  record();

  bool noEventLoop = settings.get<bool>("no-event-loop");
  if ((window || recorder) && !noEventLoop) {
    return exec();
  } else {
    return 0;
  }
}

/**
 * @brief Print the help message if the help flag is set.
 */
bool App::printHelp() {
  bool help = settings.get<bool>("help");
  if (help) {
    std::cout << parser.help() << std::endl;
  }
  return help;
}

/**
 * @brief enableDebugMode
 *
 * If the debug flag is set, enable the gui and set the camera to the debug
 * video.
 */
void App::debug() {
  if (settings.get<bool>("debug")) {
    settings.put("gui", true);
    settings.put("camera", debug_video.string());
  }
}

/**
 * @brief printCameras
 *
 * Print the available cameras that are found by Qt's QMediaDevices module to
 * stdout.
 *
 * @return exit code
 */
void App::list() {
  if (settings.get<bool>("list")) {
    std::cout << listCameras() << std::endl;
  }
}

/**
 * @brief connectCamera
 *
 * Connect to the camera specified in the config file, if it is found.
 */
void App::connect() {
  std::string id = settings.get<std::string>("camera");
  std::string timeout = settings.get<std::string>("timeout");
  auto optional = videoFactory(id, stringToSec(timeout));

  if (optional.has_value()) {
    video = std::move(optional.value());
    video->start();
  } else {
    spdlog::info("No video found.");
  }
}

/**
 * @brief show
 *
 * Show the GUI.
 *
 * @return exit code
 */
void App::show() {
  if (!settings.get<bool>("gui")) {
    return;
  }

  window = std::make_unique<MainWindow>();
  if (video) {
    window->scene.setVideo(video);
  }
  window->show();
}

/**
 * @brief record
 *
 * Record a video.
 *
 * @return exit code
 */
void App::record() {
  if (!settings.get<bool>("record")) {
    return;
  }

  path path_save(Path::expand(settings.get<std::string>("folder")));
  QVideoSink *sink_ptr = video->getVideoSink();
  recorder = std::make_unique<Recorder>(sink_ptr, path_save);

  sec duration = stringToSec(settings.get<std::string>("duration"));
  sec interval = stringToSec(settings.get<std::string>("interval"));
  std::string maxBytesString = settings.get<std::string>("max-bytes");
  uint64_t maxBytes = Config::scientificToUint64(maxBytesString);
  recorder->start(ms(interval), ms(duration), ms(1000), maxBytes);
}

/**
 * @brief getSettings
 *
 * Returns the settings.
 *
 * @return The settings.
 */
ptree App::getSettings() { return settings; }
