#pragma once

#include "EventSender.hpp"
#include "ctrl/BookmarkManagerEvent.hpp"
#include <memory>

namespace core
{
class BookmarkNode_Root;
class BookmarkNode;
} // namespace core

namespace ctrl
{

class BookmarkManager final : BookmarkManagerEventParam
{
public:
    BookmarkManager();

    auto GetRootBookmark() const -> std::shared_ptr<core::BookmarkNode_Root>;
    auto GetRootBookmarkBase() const -> std::shared_ptr<core::BookmarkNode>;

    auto SetCurrentNode(std::shared_ptr<core::BookmarkNode> currentNode) -> bool;
    auto GetCurrentNode() const -> std::shared_ptr<core::BookmarkNode>;

    EventSender<BookmarkManagerEvent> eventSender;

private:
    const std::shared_ptr<core::BookmarkNode_Root> m_rootBookmark;
    std::shared_ptr<core::BookmarkNode> m_currentNode;
};

} // namespace ctrl
