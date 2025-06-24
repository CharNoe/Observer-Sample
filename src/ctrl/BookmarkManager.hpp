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

class BookmarkManager final : public EventSender<BookmarkManagerEvent>
{
public:
    BookmarkManager();

    auto GetRootBookmark() const -> std::shared_ptr<core::BookmarkNode_Root>;
    auto GetRootBookmarkBase() const -> std::shared_ptr<core::BookmarkNode>;

    auto SetCurrentNode(std::shared_ptr<core::BookmarkNode> currentNode) -> bool;
    auto GetCurrentNode() const -> std::shared_ptr<core::BookmarkNode>;

    auto SetSelectNodes(std::vector<std::shared_ptr<core::BookmarkNode>> selectNodes)
        -> bool;
    auto GetSelectNodes() const
        -> const std::vector<std::shared_ptr<core::BookmarkNode>>&;

    auto DeleteCurrentNode() -> bool;
    auto DeleteSelectNodes() -> size_t;

    void CallReceiveEvent(BookmarkManagerEvent& receiver) const;

private:
    const std::shared_ptr<core::BookmarkNode_Root> m_rootBookmark;
    std::shared_ptr<core::BookmarkNode> m_currentNode;
    std::vector<std::shared_ptr<core::BookmarkNode>> m_selectNodes;
};

} // namespace ctrl
