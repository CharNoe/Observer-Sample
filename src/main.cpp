#include "gui/MainWindow.hpp"

#include "ctrl/System.hpp"
#include <QApplication>

auto main(int argc, char* argv[]) -> int
{
    QApplication a(argc, argv);
    ctrl::System system;
    gui::MainWindow w{system};
    w.show();
    return a.exec();
}
