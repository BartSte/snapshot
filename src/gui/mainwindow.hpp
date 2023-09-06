#pragma once

#include <QCameraDevice>
#include <QMainWindow>
#include <string>

#include "./gui/videoscene.hpp"
#include "./ui_mainwindow.h"

class MainWindow : public QMainWindow, public Ui::MainWindow {

 public:
  VideoScene scene;

  explicit MainWindow(QWidget *parent = nullptr);

 protected:
  virtual void resizeEvent(QResizeEvent *event);
};
