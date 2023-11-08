#include <boost/dll.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cxxopts.hpp>
#include <helpers/config.hpp>
#include <helpers/logger.hpp>
#include <iostream>
#include <qapplication.h>
#include <string>
#include <video/find.hpp>
#include <video/record.hpp>

#include "./app.hpp"
#include "./gui.hpp"

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
  initLogger(settings);
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
 * @brief initLogger
 *
 * Initialize the logger.
 *
 * @param args The cli arguments
 */
void App::initLogger(const ptree &config) {
  logging::set(settings.get<std::string>("loglevel"),
               settings.get<std::string>("pattern"));
}

/**
 * @brief exec
 *
 * Executes the program.
 *
 * @return exit code
 */
int App::run() {
  printHelp();
  enableDebugMode();
  list();
  showGui();
  startRecorder();
  return exec();
}

/**
 * @brief TODO
 */
void App::printHelp() {
  if (settings.get<bool>("help")) {
    std::cout << parser.help() << std::endl;
    exit(0);
  }
}

/**
 * @brief TODO
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
 * @brief show
 *
 * Show the GUI.
 *
 * @return exit code
 */
void App::showGui() {
  if (settings.get<bool>("gui")) {
    gui = std::make_unique<Gui>();
    gui->setVideo(settings.get<std::string>("camera"));
    gui->show();
  }
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
  if (gui) {
    sink = gui->window.scene.videoItem.videoSink();
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
