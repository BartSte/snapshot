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
 * @brief handles exceptions
 *
 * Exceptions are caught and logged as errors.
 *
 * @return the exit code
 */
int App::exec() {
  try {
    return run();
  } catch (std::exception &e) {
    spdlog::error("{}", e.what());
    return 1;
  }
}

/**
 * @brief exec
 *
 * Executes the program.
 *
 * @return exit code
 */
int App::run() {
  settings = getSettings();

  setUpLogger(settings.get<std::string>("loglevel"),
              settings.get<std::string>("pattern"));

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
    return QApplication::exec();
  } else {
    return 0;
  }
}

/**
 * @brief getSettings
 *
 * Returns the settings.
 *
 * @return The settings.
 */
ptree App::getSettings() {
  if (settings.empty()) {
    settings = parseSettings();
  }
  return settings;
}

/**
 * @brief Combines the user config file, the defaults, and the cli arguments
 * into 1 ptree.
 *
 * If the user config supplies a custom config file path, an exeption is thrown
 * when the file does not exist. After parsing, the config is checked for
 * validity.
 *
 * @return The settings.
 */
ptree App::parseSettings() {
  cxxopts::ParseResult args = parser.parse();

  std::string pathConfig = args["config"].as<std::string>();
  ptree defaults = ArgParse::asPtree(args.defaults());
  ptree cli = ArgParse::asPtree(args.arguments());
  bool customConfigPath(cli.count("config"));
  ptree configUser = Config::parse(pathConfig, customConfigPath);

  ptree result = Config::merge(defaults, configUser);
  result = Config::merge(result, cli);

  ArgParse::check(result);

  return result;
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
  uint64_t maxBytes = static_cast<uint64_t>(std::stod(maxBytesString));

  recorder->start(ms(interval), ms(duration), ms(1000), maxBytes);
}
