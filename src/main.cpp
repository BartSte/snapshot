#include "./argparse.h"
#include "./mainwindow.h"
#include <QApplication>
#include <cxxopts.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
  ArgParse parser(argc, argv);
  cxxopts::ParseResult args = parser.parse();

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
