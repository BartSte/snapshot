#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <filesystem>
#include <gui/mainwindow.hpp>
#include <helpers/argparse.hpp>
#include <memory>
#include <qapplication.h>
#include <string>
#include <video/connect.hpp>
#include <video/record.hpp>

class App : public QApplication {

  using ptree = boost::property_tree::ptree;
  using path = std::filesystem::path;

 public:
  static const path root;
  static const path static_dir;
  static const path debug_video;

  App(int argc, char *argv[]);
  ptree getSettings();
  int run();

 private:
  ArgParse parser;
  ptree settings;
  std::unique_ptr<Recorder> recorder;
  std::shared_ptr<BaseVideo> video;
  std::unique_ptr<MainWindow> window;

  ptree parseConfig(const std::string &path_config, const ptree &defaults,
                    const ptree &cli);
  void setUpLogger(std::string level, std::string pattern);
  bool printHelp();
  void debug();
  void list();
  void connect();
  void record();
  void show();
  static void sigintHandler(int signal);
};
