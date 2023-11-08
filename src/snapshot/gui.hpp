#pragma once

#include <qmetaobject.h>
#include <qobject.h>
#include <string>

#include "gui/mainwindow.hpp"
#include "video/connect.hpp"

class Gui : public QObject {
 private:
  static const char *noCameraText;
  static const char *pixmapPath;

 public:
  explicit Gui(QObject *parent = nullptr);
  void setVideo(std::unique_ptr<BaseVideo> camera);
  void show();

  MainWindow window;
};
