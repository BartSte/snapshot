#pragma once

#include <qapplication.h>
#include <qmetaobject.h>
#include <string>

#include "gui/mainwindow.hpp"

class Gui : public QApplication {
 private:
  static const char *noCameraText;
  static const char *pixmapPath;

 public:
  Gui(int argc, char *argv[]);
  void setVideo(const std::string &camera);
  void show();

  MainWindow window;
};
