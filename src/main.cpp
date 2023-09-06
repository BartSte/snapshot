#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <QApplication>
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <camera/connect.hpp>
#include <camera/find.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <memory>
#include <qcameradevice.h>
#include <spdlog/spdlog.h>

#include "./gui/mainwindow.hpp"
#include "./helpers/argparse.hpp"
#include "./helpers/config.hpp"
#include "./helpers/logger.hpp"

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

extern const fs::path ROOT = boost::dll::program_location().parent_path();

const char *noCameraText = "No camera available";
const char *pixmapPath = ":/disconnected.png";
const fs::path PATH_CONFIG = (ROOT / "static/config.json");

/**
 * @brief getConfig
 *
 * Get the config from the config file and the cli arguments.
 *
 * @param path_config The path to the config file
 * @param args The cli arguments
 * @return The config
 */
const pt::ptree getConfig(const cxxopts::ParseResult &args) {
  const std::string path_default = PATH_CONFIG.string();
  const std::string path_user = args["config"].as<std::string>();
  pt::ptree config = config::parseUserDefault(path_user, path_default);
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
void initLogger(const pt::ptree &config) {
  std::string loglevel = config.get<std::string>("loglevel");
  std::string pattern = config.get<std::string>("pattern");
  logging::set(loglevel, pattern);
}

/**
 * @brief printAvailableCameras
 *
 * Print the available cameras that are found by Qt's QMediaDevices module to
 * stdout.
 *
 * @return exit code
 */
int printAvailableCameras(int argc, char *argv[]) {
  QApplication app(argc, argv);
  std::cout << listCameras() << std::endl;
  return 0;
}

/**
 * @brief showGui
 *
 * Show the GUI.
 *
 * @param argc The number of arguments
 * @param argv The cli arguments
 *
 * @return exit code
 */
int showGui(int argc, char *argv[], const pt::ptree &config) {
  QApplication app(argc, argv);

  MainWindow window;
  window.scene.setPixmap(pixmapPath);
  window.scene.setText(noCameraText);
  window.scene.setVideo(config.get<std::string>("camera"));
  window.show();

  return app.exec();
}

int main(int argc, char *argv[]) {
  ArgParse parser(argc, argv);
  cxxopts::ParseResult args = parser.parse();
  const pt::ptree config = getConfig(args);

  initLogger(config);

  if (config.get<bool>("help")) {
    std::cout << parser.help() << std::endl;
    return 0;

  } else if (config.get<bool>("list")) {
    return printAvailableCameras(argc, argv);

  } else if (config.get<bool>("gui")) {
    return showGui(argc, argv, config);

  } else {
    return 0;
  }
}
