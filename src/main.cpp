#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <QApplication>
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

#include "./gui/mainwindow.hpp"
#include "./helpers/argparse.hpp"
#include "./helpers/config.hpp"
#include "./helpers/logger.hpp"
#include "./list.hpp"

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

  const std::string path_default_config =
      (root / "static/config.json").string();
  const std::string path_user_config = args["config"].as<std::string>();
  pt::ptree config = parseConfigs(path_user_config, path_default_config);

  if (args.count("help")) {
    std::cout << parser.help() << std::endl;
    return 0;

  } else if (args.count("list")) {
    return printAvailableCameras(argc, argv);

  } else if (args.count("gui")) {
    return showGui(argc, argv);
  }
}
