#pragma once

#include <filesystem>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <helpers/argparse.hpp>
#include <memory>
#include <qapplication.h>
#include <qvideosink.h>
#include <video/record.hpp>
#include <gui/mainwindow.hpp>

#include "video/connect.hpp"

class App : public QApplication {

  using ptree = boost::property_tree::ptree;
  using path = std::filesystem::path;

 public:
  App(int argc, char *argv[]);
  int run();

 private:
  static const path ROOT;
  static const path PATH_CONFIG;
  static const path DEBUG_VIDEO;

  std::unique_ptr<Recorder> recorder;
  std::unique_ptr<BaseVideo> video;
  std::unique_ptr<MainWindow> window;
  std::unique_ptr<QVideoSink> sink;
  ArgParse parser;
  ptree settings;

  ptree parseConfig(const cxxopts::ParseResult &args);
  bool printHelp();
  void enableDebugMode();
  void list();
  void connectVideo();
  void startRecorder();
  void showGui();
};
