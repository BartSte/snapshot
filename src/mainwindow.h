#pragma once

#include "./ui_mainwindow.h"
#include "./videoscene.h"
#include <QMainWindow>

class MainWindow : public QMainWindow, public Ui::MainWindow {
  Q_OBJECT

 public:
  VideoScene *scene;

  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
};
