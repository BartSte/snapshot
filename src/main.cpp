#include "./argparse.h"
#include "./mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  ArgParse parser(argc, argv);
  QApplication app(argc, argv);
  MainWindow window;

  window.show();
  return app.exec();
}
