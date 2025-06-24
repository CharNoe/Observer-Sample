#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "ctrl/BookmarkManager.hpp"
#include "ctrl/System.hpp"
#include "gui/BookmarkToolBar.hpp"
#include "gui/BookmarkTreeWidget.hpp"
#include <QApplication>

namespace gui {

MainWindow::MainWindow(const ctrl::System& system, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_system{system}
{
    ui->setupUi(this);
    setCentralWidget(new BookmarkTreeWidget{m_system.GetBookmarkManager()});
    {
        auto toolBar = new BookmarkToolBar{m_system.GetBookmarkManager(), this};
        ui->menu_View->addAction(toolBar->toggleViewAction());
        addToolBar(Qt::TopToolBarArea, toolBar);
    }

    ConnectQt(*system.GetBookmarkManager(), this);
    system.GetBookmarkManager()->CallReceiveEvent(*this);
    connect(ui->actionAbout_Qt, &QAction::triggered, &QApplication::aboutQt);

    const auto connectAction = [this](QAction* action, auto func)
    {
        connect(action, &QAction::triggered, this, func);
    };
    connectAction(ui->actionDelete_Current, &MainWindow::DeleteCurrentBookmark);
    connectAction(ui->actionDelete_Select, &MainWindow::DeleteSelectBookmark);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DeleteCurrentBookmark()
{
    m_system.GetBookmarkManager()->DeleteCurrentNode();
}

void MainWindow::DeleteSelectBookmark()
{
    m_system.GetBookmarkManager()->DeleteSelectNodes();
}

void MainWindow::ReceiveEvent(const BookmarkManager_CurrentChanged& param)
{
    ui->actionDelete_Current->setEnabled(static_cast<bool>(param.currentNode));
}

void MainWindow::ReceiveEvent(const BookmarkManager_SelectChanged& param)
{
    ui->actionDelete_Select->setEnabled(!param.selectNodes.empty());
}

} // namespace gui
