#pragma once

#include "./ui_mainwindow.h"
#include "./videoscene.h"
#include <QMainWindow>
#include <boost/optional.hpp>

class MainWindow : public QMainWindow, public Ui::MainWindow {
  Q_OBJECT

 public:
  VideoScene scene;

  explicit MainWindow(QWidget *parent = nullptr);

 protected:
  void resizeEvent(QResizeEvent *event);
};

boost::optional<const QCameraDevice> getCameraDevice();
