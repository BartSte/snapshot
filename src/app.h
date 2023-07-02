#include <QMainWindow>
#include "./mainwindow.h"

class AppWindow : public QMainWindow, public Ui::MainWindow {
    Q_OBJECT

public:
    AppWindow(QWidget *parent = nullptr);
    ~AppWindow();
};
