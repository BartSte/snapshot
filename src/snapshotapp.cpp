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

const path App::root = program_location();
const path App::static_dir = App::root / ".." / "static";
const path App::path_config = App::static_dir / "config.json";
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
  auto args = parser.parse();
  settings = parseConfig(args);
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
    App::instance()->quit();
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
 * @brief getConfig
 *
 * Parses the config file and merges it with the cli arguments.
 *
 * @param args The cli arguments
 * @return The config stored in a boost::property_tree::ptree
 */
ptree App::parseConfig(const cxxopts::ParseResult &args) {
  const std::string path_default = App::path_config.string();
  const std::string path_user = args["config"].as<std::string>();

  ptree config = config::parseUserDefault(path_user, path_default);
  config::merge(config, args);

  return config;
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
  if (window || recorder) {
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

  path path_save(settings.get<std::string>("folder"));
  QVideoSink *sink_ptr = video->getVideoSink();
  recorder = std::make_unique<Recorder>(sink_ptr, path_save);

  sec duration = stringToSec(settings.get<std::string>("duration"));
  sec interval = stringToSec(settings.get<std::string>("interval"));
  recorder->start(ms(interval), ms(duration));
}
