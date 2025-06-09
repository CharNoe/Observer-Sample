#pragma once

#include <QPointer>
#include <QWidget>

namespace core
{
class BookmarkNode;
} // namespace core
namespace ctrl
{
class BookmarkItemModelTree;
class BookmarkManager;
} // namespace ctrl

namespace gui {

namespace Ui {
class BookmarkTreeWidget;
}

class BookmarkTreeWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit BookmarkTreeWidget(
        std::shared_ptr<ctrl::BookmarkManager> bookmarkManager, QWidget* parent = nullptr
    );
    ~BookmarkTreeWidget();

private:
    void AddBookmarkUrl();
    void AddBookmarkFolder();

    Ui::BookmarkTreeWidget* ui;
    QPointer<ctrl::BookmarkItemModelTree> m_model;
};

} // namespace gui
