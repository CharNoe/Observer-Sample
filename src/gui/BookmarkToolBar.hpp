#pragma once

#include "core/BookmarkNodeEvent.hpp"
#include "core/BookmarkNode_UrlEvent.hpp"
#include <QMenu>
#include <QToolBar>

namespace core
{
class BookmarkNode_Url;
} // namespace core
namespace ctrl
{
class BookmarkManager;
} // namespace ctrl

namespace gui
{

class BookmarkToolBar final
    : public QToolBar
    , public core::BookmarkNodeEvent
{
public:
    BookmarkToolBar(
        std::shared_ptr<ctrl::BookmarkManager> bookmarkManager, QWidget* parent = nullptr
    );

private:
    class FolderMenu;
    class UrlAction;
    class ActionFactory;

    static auto MakeAction(std::shared_ptr<core::BookmarkNode> node, QWidget* parent)
        -> QAction*;
    void PushAction();

    const std::shared_ptr<ctrl::BookmarkManager> m_bookmarkManager;
    size_t m_childrenSize;

    // BookmarkNodeEvent interface
private:
    void ReceiveEvent(const BookmarkNode_ChildInserted& param) override;
    void ReceiveEvent(const BookmarkNode_ChildErased& param) override;
};

class BookmarkToolBar::FolderMenu final
    : public QMenu
    , public core::BookmarkNodeEvent
{
public:
    FolderMenu(std::shared_ptr<core::BookmarkNode> node, QWidget* parent);

private:
    size_t m_childrenSize;

    // BookmarkNodeEvent interface
private:
    void ReceiveEvent(const BookmarkNode_NameChanged& param) override;
    void ReceiveEvent(const BookmarkNode_ChildInserted& param) override;
    void ReceiveEvent(const BookmarkNode_ChildErased& param) override;
};

class BookmarkToolBar::UrlAction final
    : public QAction
    , public core::BookmarkNodeEvent
    , public core::BookmarkNode_UrlEvent
{
public:
    UrlAction(std::shared_ptr<core::BookmarkNode_Url> node, QObject* parent);

private:
    void OnTriggered();

    QString m_url;

    // BookmarkNodeEvent interface
private:
    void ReceiveEvent(const BookmarkNode_NameChanged& param) override;

    // BookmarkNode_UrlEvent interface
public:
    void ReceiveEvent(const BookmarkNode_Url_UrlChanged& param) override;
};

} // namespace gui
