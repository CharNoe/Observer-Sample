#pragma once

#include "ctrl/BookmarkManagerEvent.hpp"
#include <QItemSelection>
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

class BookmarkTreeWidget final
    : public QWidget
    , public ctrl::BookmarkManagerEvent
{
    Q_OBJECT

public:
    explicit BookmarkTreeWidget(
        std::shared_ptr<ctrl::BookmarkManager> bookmarkManager, QWidget* parent = nullptr
    );
    ~BookmarkTreeWidget();

private:
    void OnCurrentIndexChanged(const QModelIndex& after, const QModelIndex& before);
    void OnSelectionIndexChanged(
        const QItemSelection& selected, const QItemSelection& deselected
    );
    void AddBookmarkUrl();
    void AddBookmarkFolder();
    void AddBookmark(std::shared_ptr<core::BookmarkNode> node);
    void DeleteSelectBookmarks();

    Ui::BookmarkTreeWidget* ui;
    const std::shared_ptr<ctrl::BookmarkManager> m_bookmarkManager;
    QPointer<ctrl::BookmarkItemModelTree> m_model;

    // BookmarkManagerEvent interface
private:
    void ReceiveEvent(const BookmarkManager_SelectChanged& param) override;
};

} // namespace gui
