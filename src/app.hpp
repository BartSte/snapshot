#pragma once

#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cxxopts.hpp>
#include <qapplication.h>
#include <qmetaobject.h>

#include "gui/mainwindow.hpp"
#include "helpers/argparse.hpp"

class App {

  using ptree = boost::property_tree::ptree;
  using path = boost::filesystem::path;

 public:
  App(int argc, char *argv[]);
  int exec();

 private:
  static const path ROOT;
  static const path PATH_CONFIG;

  int argc;
  char **argv;
  ArgParse parser;
  ptree settings;

  ptree parseConfig(const cxxopts::ParseResult &args);
  void initLogger(const ptree &config);
  int printCameras();
  int show();
};
