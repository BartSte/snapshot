#pragma once

#include <QGraphicsPixmapItem>
#include <qgraphicsscene.h>
#include <string>

class VideoScene : public QGraphicsScene {
 public:
  const std::string pathImageDisconnected =
      "/home/barts/code/snapshot/src/static/disconnected.png";

  QGraphicsTextItem *textItem;
  QGraphicsPixmapItem *pixmapItem;

  explicit VideoScene(QWidget *mainWindow);
  ~VideoScene();
};
