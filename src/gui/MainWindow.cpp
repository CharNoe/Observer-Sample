#include "MainWindow.hpp"
#include "ctrl/BookmarkManager.hpp"
#include "ui_MainWindow.h"

#include "ctrl/System.hpp"
#include "gui/BookmarkToolBar.hpp"
#include "gui/BookmarkTreeWidget.hpp"

namespace gui {

MainWindow::MainWindow(const ctrl::System& system, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_system{system}
{
    ui->setupUi(this);
    setCentralWidget(new BookmarkTreeWidget{m_system.GetBookmarkManager()});
    addToolBar(
        Qt::TopToolBarArea,
        new BookmarkToolBar{m_system.GetBookmarkManager()->GetRootBookmarkBase(), this}
    );

    system.GetBookmarkManager()->eventSender.ConnectQt(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ReceiveEvent(const ctrl::BookmarkManagerEventParam::CurrentChanged& param
)
{
    ui->actionDelete_Current->setEnabled(static_cast<bool>(param.currentNode));
}

} // namespace gui
