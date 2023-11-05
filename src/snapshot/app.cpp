#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cxxopts.hpp>
#include <helpers/config.hpp>
#include <helpers/logger.hpp>
#include <iostream>
#include <qapplication.h>
#include <string>
#include <video/find.hpp>

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
App::App(int argc, char *argv[]) : argc(argc), argv(argv), parser(argc, argv) {
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
int App::exec() {
  if (settings.get<bool>("help")) {
    std::cout << parser.help() << std::endl;
    return 0;
  } else if (settings.get<bool>("debug")) {
    settings.put("gui", true);
    settings.put("camera", DEBUG_VIDEO.string());
  }

  int exit_code = 0;
  if (settings.get<bool>("list")) {
    exit_code = printCameras();
  }

  if (settings.get<bool>("gui")) {
    exit_code = show();
  }

  return exit_code;
}

/**
 * @brief printCameras
 *
 * Print the available cameras that are found by Qt's QMediaDevices module to
 * stdout.
 *
 * @return exit code
 */
int App::printCameras() {
  QApplication app(argc, argv);
  std::cout << listCameras() << std::endl;
  return 0;
}

/**
 * @brief show
 *
 * Show the GUI.
 *
 * @return exit code
 */
int App::show() {
  Gui gui(argc, argv);
  gui.setVideo(settings.get<std::string>("camera"));
  gui.show();
  return gui.exec();
}
