#pragma once

#include <QGraphicsPixmapItem>
#include <qgraphicsscene.h>
#include <qsize.h>
#include <string>

class VideoScene : public QGraphicsScene {
  const float constRatio;
  const float textOffset;
  const float textFontSize;

  const QString textFont;

 public:
  QGraphicsTextItem *textItem;
  QGraphicsPixmapItem *pixmapItem;

  explicit VideoScene(QWidget *mainWindow);
  void setPixmap(std::string path);
  void scalePixmap();
  void centerPixmap();
  void setText(std::string text);
  void setPosText();

  ~VideoScene();
};
