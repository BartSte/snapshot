#include "./argparse.h"
#include "./mainwindow.h"
#include <QApplication>
#include <cxxopts.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
  ArgParse parser(argc, argv);
  cxxopts::ParseResult args = parser.parse();
  spdlog::set_level(spdlog::level::debug);
  spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

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
