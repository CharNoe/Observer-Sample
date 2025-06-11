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
    void OnCurrentIndexChanged(const QModelIndex& after, const QModelIndex& before);
    void AddBookmarkUrl();
    void AddBookmarkFolder();
    void AddBookmark(std::shared_ptr<core::BookmarkNode> node);

    Ui::BookmarkTreeWidget* ui;
    const std::shared_ptr<ctrl::BookmarkManager> m_bookmarkManager;
    QPointer<ctrl::BookmarkItemModelTree> m_model;
};

} // namespace gui
