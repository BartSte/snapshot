#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "./argparse.hpp"
#include "./list.hpp"
#include "./logger.hpp"
#include "./mainwindow.hpp"
#include <QApplication>
#include <cxxopts.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

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
void readConfig(std::string path) {
  SPDLOG_DEBUG("Reading config file from {}", path);
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
  readConfig(args["config"].as<std::string>());

  if (args.count("help")) {
    std::cout << parser.help() << std::endl;
    return 0;

  } else if (args.count("list")) {
    return printAvailableCameras(argc, argv);

  } else if (args.count("gui")) {
    return showGui(argc, argv);
  }
}
