#include "app.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AppWindow window;
    VideoViewer viewer(window.graphicsView);

    window.show();
    return app.exec();
}
