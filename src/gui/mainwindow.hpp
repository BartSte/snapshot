#pragma once

#include <QCameraDevice>
#include <QMainWindow>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <string>

#include "./gui/videoscene.hpp"
#include "./ui_mainwindow.h"

class MainWindow : public QMainWindow, public Ui::MainWindow {

 public:
  VideoScene scene;
  QCameraDevice cameraDevice;
  QString url;

  explicit MainWindow(QWidget *parent = nullptr);
  void setVideo(const std::string &id);
  void updateScene();

  virtual ~MainWindow();

 protected:
  void setCamera(const QCameraDevice &cameraDevice);
  void setStream(const QString &url);
  virtual void resizeEvent(QResizeEvent *event);
};
