#include <boost/dll.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cxxopts.hpp>
#include <gui/mainwindow.hpp>
#include <helpers/config.hpp>
#include <helpers/logger.hpp>
#include <iostream>
#include <qapplication.h>
#include <string>
#include <video/find.hpp>
#include <video/record.hpp>

#include "./app.hpp"

using ptree = boost::property_tree::ptree;
using path = boost::filesystem::path;

const path App::ROOT = boost::dll::program_location().parent_path();
const path App::PATH_CONFIG = App::ROOT / "static/config.json";
const path App::DEBUG_VIDEO = App::ROOT / "static/sample.mp4";

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
  auto args = parser.parse();
  settings = parseConfig(args);
  logging::set(settings.get<std::string>("loglevel"),
               settings.get<std::string>("pattern"));
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
  const std::string path_default = App::PATH_CONFIG.string();
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
  enableDebugMode();
  list();
  connectCamera();
  showGui();
  startRecorder();
  return exec();
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
void App::enableDebugMode() {
  if (settings.get<bool>("debug")) {
    settings.put("gui", true);
    settings.put("camera", DEBUG_VIDEO.string());
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
void App::connectCamera() {
  std::string id = settings.get<std::string>("camera");
  auto optional = videoFactory(id);
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
void App::showGui() {
  if (!settings.get<bool>("gui")) {
    return;
  }

  window = std::make_unique<MainWindow>();
  if (video) {
    window->scene.setVideo(std::move(video));
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
void App::startRecorder() {
  if (!settings.get<bool>("record")) {
    return;
  }
  QVideoSink *sink;
  if (window) {
    sink = window->scene.videoItem.videoSink();
    recorder = std::make_unique<Recorder>(sink);
  } else {
    spdlog::error("Recorder without gui not implemented.");
    /* The gui holds on to the BaseVideo object. If there is no GUI, there is
     * also no BaseVideo object. A solution could be to create a BaseVideo
     * on she heap and move it to the GUI when it is created. If no GUI is
     * created, the App holds on to the BaseVideo object and passes it
     * QVideoSink to the Recorder. */
    /* recorder = std::make_unique<Recorder>() */
  }
}
