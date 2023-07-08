#pragma once

#include <QGraphicsPixmapItem>
#include <qgraphicsscene.h>
#include <string>

class VideoScene : public QGraphicsScene {
  const float constRatio;

 public:
  QGraphicsTextItem *textItem;
  QGraphicsPixmapItem *pixmapItem;

  explicit VideoScene(QWidget *mainWindow);
  void setPixmap(std::string path);
  void scalePixmap();
  void centerPixmap();
  void setText(std::string text);
  ~VideoScene();
};
