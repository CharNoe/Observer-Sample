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
    Ui::MainWindow *ui;
    const ctrl::System& m_system;

    // BookmarkManagerEvent interface
private:
    void ReceiveEvent(const ctrl::BookmarkManagerEventParam::CurrentChanged& param
    ) override;
};

} // namespace gui
