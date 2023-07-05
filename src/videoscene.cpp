#include "./videoscene.h"
#include <iostream>
#include <qgraphicsscene.h>
#include <qwidget.h>

VideoScene::VideoScene(QWidget *mainWindow)
    : QGraphicsScene(mainWindow),
      pixmapItem(new QGraphicsPixmapItem()),
      textItem(new QGraphicsTextItem()) {

  QString qpath = QString::fromStdString(pathImageDisconnected);
  QPixmap pixmap(qpath);
  pixmapItem->setPixmap(pixmap);
  addItem(pixmapItem);

  QString text = QString::fromStdString("Disconnected");
  textItem->setPlainText(text);
  addItem(textItem);
}

VideoScene::~VideoScene() {
  delete pixmapItem;
  delete textItem;
}
