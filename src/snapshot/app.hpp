#pragma once

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <helpers/argparse.hpp>
#include <memory>
#include <qapplication.h>
#include <video/record.hpp>

#include "snapshot/gui.hpp"
#include "video/connect.hpp"

class App : public QApplication {

  using ptree = boost::property_tree::ptree;
  using path = boost::filesystem::path;

 public:
  App(int argc, char *argv[]);
  int run();

 private:
  static const path ROOT;
  static const path PATH_CONFIG;
  static const path DEBUG_VIDEO;

  std::unique_ptr<Gui> gui;
  std::unique_ptr<Recorder> recorder;
  std::unique_ptr<BaseVideo> video;
  ArgParse parser;
  ptree settings;

  ptree parseConfig(const cxxopts::ParseResult &args);
  void initLogger(const ptree &config);
  void printHelp();
  void enableDebugMode();
  void list();
  void connectCamera();
  void startRecorder();
  void showGui();
};
