#include <QMainWindow>
#include "./mainwindow.h"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow(QWidget *parent = nullptr);
    ~AppWindow();

private:
    Ui::MainWindow *ui;
};
