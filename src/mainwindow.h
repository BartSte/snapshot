#pragma once

#include "ui_mainwindow.h"
#include <QMainWindow>

class AppWindow : public QMainWindow, public Ui::MainWindow {
    Q_OBJECT

public:
    AppWindow(QWidget *parent = nullptr);
    ~AppWindow();
};
