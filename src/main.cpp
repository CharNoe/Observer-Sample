#include "gui/MainWindow.hpp"

#include "ctrl/System.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ctrl::System system;
    gui::MainWindow w{system};
    w.show();
    return a.exec();
}
