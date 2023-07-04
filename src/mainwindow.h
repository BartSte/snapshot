#pragma once

#include "./ui_mainwindow.h"
#include "./viewer.h"
#include <QMainWindow>

class MainWindow : public QMainWindow, public Ui::MainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  VideoViewer viewer;

  ~MainWindow();
};
