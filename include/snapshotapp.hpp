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
  App(int &argc, char *argv[]);
  ptree getSettings();
  int exec();

 public slots:
  void quitWhenHeadless();

 private:
  ArgParse parser;
  ptree settings;
  std::unique_ptr<Recorder> recorder;
  std::shared_ptr<BaseVideo> video;
  std::unique_ptr<MainWindow> window;

  static void sigintHandler(int signal);

  int start();
  ptree parseSettings();
  void setUpLogger(std::string level, std::string pattern);
  bool printHelp();
  bool version();
  bool list();
  void makeVideo();
  void startVideo();
  void makeRecorder();
  void startRecorder();
  void makeGui();
  void startGui();
  int execWithFlag(bool disable);

  void stop();
  bool videoActive();
  bool recorderActive();
  bool guiActive();
};
