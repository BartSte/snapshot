#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <QApplication>
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <memory>
#include <qcameradevice.h>
#include <spdlog/spdlog.h>

#include "./camera.hpp"
#include "./gui/mainwindow.hpp"
#include "./helpers/argparse.hpp"
#include "./helpers/config.hpp"
#include "./helpers/logger.hpp"
#include "./list.hpp"

namespace pt = boost::property_tree;
namespace fs = boost::filesystem;

extern const fs::path root = boost::dll::program_location().parent_path();
const fs::path path_config = (root / "static/config.json");

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
int showGui(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow window;

  boost::optional<QCameraDevice> cameraDevice = selectCamera();
  if (cameraDevice) {
    window.setCameraDevice(*cameraDevice);
  }

  window.show();
  return app.exec();
}

int main(int argc, char *argv[]) {
  ArgParse parser(argc, argv);
  cxxopts::ParseResult args = parser.parse();

  initLogger(args);

  const std::string path_default = path_config.string();
  const std::string path_user = args["config"].as<std::string>();
  pt::ptree config = config::parseUserDefault(path_user, path_default);

  std::unique_ptr<pt::ptree> pconfig = std::make_unique<pt::ptree>(config);
  pconfig = config::merge(std::move(pconfig), args);

  if (pconfig->count("help")) {
    std::cout << parser.help() << std::endl;
    return 0;

  } else if (pconfig->count("list")) {
    return printAvailableCameras(argc, argv);

  } else if (pconfig->count("gui")) {
    return showGui(argc, argv);
  }
}
