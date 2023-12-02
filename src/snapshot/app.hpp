#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <filesystem>
#include <gui/mainwindow.hpp>
#include <helpers/argparse.hpp>
#include <memory>
#include <qapplication.h>
#include <qvideosink.h>
#include <video/record.hpp>

#include "video/connect.hpp"

class App : public QApplication {

  using ptree = boost::property_tree::ptree;
  using path = std::filesystem::path;

 public:
  App(int argc, char *argv[]);
  ptree parseConfig(const cxxopts::ParseResult &args);
  bool printHelp();
  void enableDebugMode();
  void list();
  void connectVideo();
  void startRecorder();
  void showGui();
  int run();

 private:
  static const path ROOT;
  static const path PATH_CONFIG;
  static const path DEBUG_VIDEO;

  std::unique_ptr<Recorder> recorder;
  std::shared_ptr<BaseVideo> video;
  std::unique_ptr<MainWindow> window;

  ArgParse parser;
  ptree settings;

  void makeVideoSink();
};
