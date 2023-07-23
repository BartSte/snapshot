#pragma once

#include "./ui_mainwindow.h"
#include "./videoscene.hpp"
#include <QMainWindow>
#include <boost/optional.hpp>

class MainWindow : public QMainWindow, public Ui::MainWindow {

 public:
  VideoScene scene;

  explicit MainWindow(QWidget *parent = nullptr);
  void updateScene();

  virtual ~MainWindow();

 protected:
  virtual void resizeEvent(QResizeEvent *event);
};
