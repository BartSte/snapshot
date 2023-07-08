#include "./videoscene.h"
#include <iostream>
#include <qgraphicsscene.h>
#include <qwidget.h>

VideoScene::VideoScene(QWidget *mainWindow)
    : QGraphicsScene(mainWindow),
      constRatio(0.8),
      pixmapItem(new QGraphicsPixmapItem()),
      textItem(new QGraphicsTextItem()) {
  setPixmap(":/images/disconnected.png");
  setText("Disconnected");
}

void VideoScene::setPixmap(std::string path) {
  QString qpath = QString::fromStdString(path);
  QPixmap pixmap(qpath);
  pixmapItem->setPixmap(pixmap);
  addItem(pixmapItem);
}

void VideoScene::scalePixmap() {
  QPixmap image = pixmapItem->pixmap();

  float widthRatio = sceneRect().width() / image.size().width();
  float heightRatio = sceneRect().height() / image.size().height();
  float aspectRatio = std::min(widthRatio, heightRatio);
  float clippedRatio = std::min(aspectRatio, 1.0f);

  pixmapItem->setScale(clippedRatio * constRatio);
}

void VideoScene::centerPixmap() {
  float scale = pixmapItem->scale();
  QPixmap image = pixmapItem->pixmap();

  float widthImage = image.size().width() * scale;
  float heightImage = image.size().height() * scale;
  float x = (sceneRect().width() - widthImage) / 2;
  float y = (sceneRect().height() - heightImage) / 2;

  pixmapItem->setPos(x, y);
}

void VideoScene::setText(std::string text) {
  QString qtext = QString::fromStdString(text);
  textItem->setPlainText(qtext);
  /* textItem->setPos(pos); */
  addItem(textItem);
}

VideoScene::~VideoScene() {
  delete pixmapItem;
  delete textItem;
}
