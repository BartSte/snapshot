#include "./argparse.h"
#include "./logger.h"
#include "./mainwindow.h"
#include <QApplication>
#include <cxxopts.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
  ArgParse parser(argc, argv);
  cxxopts::ParseResult args = parser.parse();

  std::string loglevel = args["loglevel"].as<std::string>();
  std::string pattern = args["pattern"].as<std::string>();
  setLogger(loglevel, pattern);

  if (args.count("help")) {
    std::cout << parser.help() << std::endl;
    return 0;
  }

  if (args.count("gui")) {
    QApplication app(argc, argv);
    MainWindow window;

    window.show();
    return app.exec();
  }
}
