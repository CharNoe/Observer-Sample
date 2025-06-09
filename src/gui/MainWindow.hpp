#pragma once

#include <QMainWindow>

namespace ctrl
{
class System;
} // namespace ctrl

namespace gui {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const ctrl::System& system, QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    const ctrl::System& m_system;
};

} // namespace gui
