#pragma once

#include <QDialog>
#include <memory>

namespace core
{
class BookmarkNode_Url;
} // namespace core

namespace gui
{

namespace Ui
{
class BookmarkUrlFactoryDialog;
}

class BookmarkUrlFactoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookmarkUrlFactoryDialog(QWidget* parent = nullptr);
    ~BookmarkUrlFactoryDialog();

    static auto Execute(QWidget* parent = nullptr)
        -> std::shared_ptr<core::BookmarkNode_Url>;

private:
    void OnTextChanged();

    Ui::BookmarkUrlFactoryDialog* ui;
};

} // namespace gui
