#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "./argparse.h"
#include "./logger.h"
#include "./mainwindow.h"
#include <QApplication>
#include <cxxopts.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

void initLogger(cxxopts::ParseResult args) {
  std::string loglevel = args["loglevel"].as<std::string>();
  std::string pattern = args["pattern"].as<std::string>();
  setLogger(loglevel, pattern);
}

int showHelp(cxxopts::ParseResult args) {
  std::cout << args["help"].as<std::string>() << std::endl;
  return 0;
}

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

  if (args.count("help")) {
    return showHelp(args);
  } else if (args.count("gui")) {
    return showGui(argc, argv);
  }
}
