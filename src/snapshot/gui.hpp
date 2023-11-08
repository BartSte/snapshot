#pragma once

#include <qmetaobject.h>
#include <qobject.h>
#include <string>

#include "gui/mainwindow.hpp"

class Gui : public QObject {
 private:
  static const char *noCameraText;
  static const char *pixmapPath;

 public:
  explicit Gui(QObject *parent = nullptr);
  void setVideo(const std::string &camera);
  void show();

  MainWindow window;
};
