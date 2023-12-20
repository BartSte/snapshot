#pragma once

#include <QCameraDevice>
#include <QMainWindow>
#include <gui/videoscene.hpp>
#include <string>

#include <ui_mainwindow.h>

class MainWindow : public QMainWindow, public Ui::MainWindow {

 protected:
  void resizeEvent(QResizeEvent *event) override;

 public:
  VideoScene scene;
  explicit MainWindow(QWidget *parent = nullptr);
};
