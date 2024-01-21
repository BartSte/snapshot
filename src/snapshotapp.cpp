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
#include <qobject.h>
#include <signal.h>
#include <spdlog/spdlog.h>
#include <string>
#include <version.h>
#include <video/find.hpp>
#include <video/record.hpp>

using ptree = boost::property_tree::ptree;
using path = std::filesystem::path;
using sec = std::chrono::seconds;
using ms = std::chrono::milliseconds;

/**
 * @brief Constructor
 *
 * Parses the cli arguments and merges them with the config file. The config is
 * used to initialize the logger, and also further in the program.
 *
 * @param argc
 * @param argv
 */
App::App(int &argc, char *argv[])
    : QApplication(argc, argv), parser(argc, argv) {
  signal(SIGINT, sigintHandler);
  QObject::connect(this, &App::aboutToQuit, this, &App::stop);
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
    QCoreApplication *app = App::instance();
    if (app == nullptr) {
      spdlog::warn("QApplication is a nullptr.");
    } else {
      spdlog::debug("Quitting QApplication");
      App::instance()->quit();
    }
  }
}

/**
 * @brief Stop the injected dependencies.
 */
void App::stop() {
  spdlog::debug("App::stop called");
  if (videoActive()) {
    video->stop();
  }
  if (recorderActive()) {
    disconnect(recorder.get(), &Recorder::stateChanged, this,
               &App::quitWhenHeadless);
    recorder->stop();
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
    return start();
  } catch (std::exception &e) {
    quit();
    spdlog::error("{}", e.what());
    return 1;
  }
}

/**
 * @brief run
 *
 * Executes the program without handling exceptions.
 *
 * @return exit code
 */
int App::start() {
  settings = getSettings();

  setUpLogger(settings.get<std::string>("loglevel"),
              settings.get<std::string>("pattern"));

  if (printHelp() || version() || list()) {
    return 0;
  }

  makeVideo();
  makeGui();
  makeRecorder();

  if (window || recorder) {
    startGui();
    startVideo();
    startRecorder();
    return execWithFlag(settings.get<bool>("no-event-loop"));
  } else {
    spdlog::warn("No gui, or recorder created. Exiting...");
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
 * If the user config supplies a custom config file path, an exeption is
 * thrown when the file does not exist. After parsing, the config is checked
 * for validity.
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
 * @brief Returns the version number that is defined in the CMakeLists.txt.
 *
 * @return The version number.
 */
bool App::version() {
  bool version = settings.get<bool>("version");
  if (version) {
    std::cout << "Version: " << PROJECT_VERSION << std::endl;
  }
  return version;
}

/**
 * @brief printCameras
 *
 * Print the available cameras that are found by Qt's QMediaDevices module to
 * stdout.
 *
 * @return exit code
 */
bool App::list() {
  bool list = settings.get<bool>("list");
  if (list) {
    std::cout << listCameras() << std::endl;
  }
  return list;
}

/**
 * @brief connectCamera
 *
 * Connect to the camera specified in the config file, if it is found.
 */
void App::makeVideo() {
  spdlog::debug("Creating video");
  std::string id = settings.get<std::string>("camera");
  std::string timeout = settings.get<std::string>("timeout");
  auto optional = videoFactory(id, stringToSec(timeout));

  if (optional.has_value()) {
    video = std::move(optional.value());
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
void App::makeGui() {
  if (!settings.get<bool>("gui")) {
    return;
  }

  spdlog::debug("Creating gui");
  window = std::make_unique<MainWindow>();
  if (video) {
    window->scene.setVideo(video);
  }
}

/**
 * @brief record
 *
 * Record a video.
 *
 * @return exit code
 */
void App::makeRecorder() {
  if (!settings.get<bool>("record")) {
    return;
  }

  spdlog::debug("Creating recorder");
  path path_save(Path::expand(settings.get<std::string>("folder")));
  QVideoSink *sink_ptr = video->getVideoSink();
  recorder = std::make_unique<Recorder>(sink_ptr, path_save);
  QObject::connect(recorder.get(), &Recorder::stateChanged, this,
                   &App::quitWhenHeadless);
}

/**
 * @brief Quit when the recorder is stopped and no gui is active.
 */
void App::quitWhenHeadless() {
  spdlog::debug("quitWhenHeadless called");
  if (recorder->getState() != RecorderState::Stop) {
    spdlog::debug("Recorder is not stopped.");
    return;
  } else if (guiActive()) {
    spdlog::debug("Gui is active");
    return;
  } else if (App::closingDown()) {
    spdlog::debug("App is already closing down");
    return;
  } else {
    spdlog::debug("Quitting");
    quit();
  }
}

/**
 * @brief If a video is created, start it.
 */
void App::startVideo() {
  if (video) {
    spdlog::debug("Starting video");
    video->start();
  }
}

/**
 * @brief Show the GUI if it is created.
 */
void App::startGui() {
  if (window) {
    spdlog::debug("Starting gui");
    window->show();
  }
}

/**
 * @brief Start the recorder if it is created.
 */
void App::startRecorder() {
  if (recorder) {
    sec duration = stringToSec(settings.get<std::string>("duration"));
    sec interval = stringToSec(settings.get<std::string>("interval"));
    std::string maxBytesString = settings.get<std::string>("max-bytes");
    uint64_t maxBytes = static_cast<uint64_t>(std::stod(maxBytesString));

    spdlog::debug("Starting recorder");
    recorder->start(ms(interval), ms(duration), ms(1000), maxBytes);
  }
}

/**
 * @brief execute the event loop if enable is true.
 *
 * @param enable a flag to enable the event loop
 * @return the exit code
 */
int App::execWithFlag(bool disable) {
  if (disable) {
    spdlog::warn("Event loop disabled");
    return 0;
  } else {
    spdlog::debug("Starting event loop");
    return QApplication::exec();
  }
}

/**
 * @brief check if the video is active.
 *
 * @return True if the video is active, false otherwise.
 */
bool App::videoActive() {
  if (video) {
    return video->getState() == VideoState::Start;
  } else {
    return false;
  }
}

/**
 * @brief check if the gui is active.
 *
 * @return True if the gui is active, false otherwise.
 */
bool App::guiActive() {
  if (window) {
    return window->isVisible();
  } else {
    return false;
  }
}

/**
 * @brief Returns true if the recorder is created and its state is Start.
 *
 * @return True if the recorder is created and its state is Start.
 */
bool App::recorderActive() {
  if (recorder) {
    return recorder->getState() == RecorderState::Start;
  } else {
    return false;
  }
}
