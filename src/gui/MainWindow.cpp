#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "ctrl/System.hpp"
#include "gui/BookmarkTreeWidget.hpp"

namespace gui {

MainWindow::MainWindow(const ctrl::System& system, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_system{system}
{
    ui->setupUi(this);
    setCentralWidget(new BookmarkTreeWidget{m_system.GetBookmarkManager()});
}

MainWindow::~MainWindow()
{
    delete ui;
}

} // namespace gui
