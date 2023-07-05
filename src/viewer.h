#pragma once

#include <QGraphicsView>
#include <QObject>
#include <qwidget.h>

class VideoViewer {
 public:
  QGraphicsView *graphicsView;
  QGraphicsScene *scene;

  explicit VideoViewer(QWidget *mainWindow, QGraphicsView *graphicsView);
  ~VideoViewer();
};
