#pragma once

#include "ctrl/BookmarkManagerEvent.hpp"
#include <QMainWindow>

namespace ctrl
{
class System;
} // namespace ctrl

namespace gui {

namespace Ui {
class MainWindow;
}

class MainWindow
    : public QMainWindow
    , public ctrl::BookmarkManagerEvent
{
    Q_OBJECT

public:
    MainWindow(const ctrl::System& system, QWidget* parent = nullptr);
    ~MainWindow();

private:
    void DeleteCurrentBookmark();
    void DeleteSelectBookmark();

    Ui::MainWindow* ui;
    const ctrl::System& m_system;

    // BookmarkManagerEvent interface
private:
    void ReceiveEvent(const BookmarkManager_CurrentChanged& param) override;
    void ReceiveEvent(const BookmarkManager_SelectChanged& param) override;
};

} // namespace gui
