#pragma once

#include <QGraphicsView>
#include <QObject>

class VideoViewer : public QObject {
  Q_OBJECT

 public:
  int x = 0;

  explicit VideoViewer(QGraphicsView *parent);
  void displayImage(QImage image);

  ~VideoViewer();
};
