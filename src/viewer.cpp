#include "./viewer.h"
#include <QGraphicsView>
#include <QObject>
#include <iostream>

VideoViewer::VideoViewer(QGraphicsView *parent) : QObject(parent) {
  std::cout << "Viewer created" << std::endl;
}

VideoViewer::~VideoViewer() { std::cout << "Viewer destroyed" << std::endl; }
