#pragma once

#include <QGraphicsPixmapItem>
#include <qgraphicsscene.h>
#include <qsize.h>
#include <string>

/**
 * @class VideoScene
 * @brief The VideoScene class is a QGraphicsScene that displays a video
 * stream. If the stream is disconnected, it displays a disconnected image with
 * a text.
 */
class VideoScene : public QGraphicsScene {

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
