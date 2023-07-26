#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "./argparse.hpp"
#include "./config.hpp"
#include "./list.hpp"
#include "./logger.hpp"
#include "./mainwindow.hpp"
#include <QApplication>
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

namespace pt = boost::property_tree;
namespace fs = boost::filesystem;

extern const fs::path root = boost::dll::program_location().parent_path();

/**
 * @brief initLogger
 *
 * Initialize the logger.
 *
 * @param args The cli arguments
 */
void initLogger(cxxopts::ParseResult args) {
  std::string loglevel = args["loglevel"].as<std::string>();
  std::string pattern = args["pattern"].as<std::string>();
  setLogger(loglevel, pattern);
}

/**
 * @brief readConfig
 *
 * Read the config file.
 *
 * @param path The path to the config file
 */
pt::ptree readConfig(std::string path_user, std::string path_default) {
  SPDLOG_DEBUG("Reading user config file from {}", path_user);
  SPDLOG_DEBUG("Reading default config file from {}", path_default);

  pt::ptree config = parseConfig(path_default);
  pt::ptree config_user = parseConfig(path_user);
  config.insert(config.end(), config_user.begin(), config_user.end());

  return config;
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
int showGui(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow window;

  window.show();
  return app.exec();
}

int main(int argc, char *argv[]) {
  ArgParse parser(argc, argv);
  cxxopts::ParseResult args = parser.parse();

  initLogger(args);

  const fs::path path_default_config = root / "static/config.ini";
  const std::string path_user_config = args["config"].as<std::string>();
  pt::ptree config = readConfig(path_user_config, path_default_config.string());

  if (args.count("help")) {
    std::cout << parser.help() << std::endl;
    return 0;

  } else if (args.count("list")) {
    return printAvailableCameras(argc, argv);

  } else if (args.count("gui")) {
    return showGui(argc, argv);
  }
}
