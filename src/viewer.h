#pragma once

#include <QGraphicsView>
#include <QObject>

class VideoViewer : public QObject {
    Q_OBJECT

public:
    VideoViewer(QGraphicsView *parent);
    ~VideoViewer();
};
