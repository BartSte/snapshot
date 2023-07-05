#include "../src/viewer.h"
#include <QGraphicsView>
#include <QObject>
#include <iostream>
#include <qgraphicsscene.h>
#include <qwidget.h>

VideoViewer::VideoViewer(QWidget *mainWindow, QGraphicsView *graphicsView)
    : scene(new QGraphicsScene(mainWindow)), graphicsView(graphicsView) {
  std::cout << "Viewer created" << std::endl;
  /* TODO: below gives segfault: */
  /* graphicsView->setScene(scene); */
}

VideoViewer::~VideoViewer() { std::cout << "Viewer destroyed" << std::endl; }
