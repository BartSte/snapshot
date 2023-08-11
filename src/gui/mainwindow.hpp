#pragma once

#include <QMainWindow>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

#include "./gui/videoscene.hpp"
#include "./ui_mainwindow.h"

class MainWindow : public QMainWindow, public Ui::MainWindow {

 public:
  VideoScene scene;
  QCameraDevice cameraDevice;
  QString url;

  explicit MainWindow(QWidget *parent = nullptr);
  void setVideo(const QCameraDevice &cameraDevice);
  void setVideo(const QString &url);
  void enableCamera(const std::string &id);
  void updateScene();

  virtual ~MainWindow();

 protected:
  virtual void resizeEvent(QResizeEvent *event);
};
