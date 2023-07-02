#pragma once

#include "mainwindow.h"
#include "viewer.h"
#include <QMainWindow>

class AppWindow : public QMainWindow, public Ui::MainWindow {
    Q_OBJECT

public:
    AppWindow(QWidget *parent = nullptr);
    ~AppWindow();
};
