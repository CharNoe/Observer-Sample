#include "BookmarkManager.hpp"

#include "core/BookmarkNode_Root.hpp"

namespace ctrl
{

BookmarkManager::BookmarkManager()
    : m_rootBookmark{std::make_shared<core::BookmarkNode_Root>()}
{
}

auto BookmarkManager::GetRootBookmark() const -> std::shared_ptr<core::BookmarkNode_Root>
{
    return m_rootBookmark;
}

auto BookmarkManager::GetRootBookmarkBase() const -> std::shared_ptr<core::BookmarkNode>
{
    return m_rootBookmark;
}

auto BookmarkManager::SetCurrentNode(std::shared_ptr<core::BookmarkNode> currentNode)
    -> bool
{
    if (m_currentNode == currentNode)
    {
        return false;
    }

    m_currentNode = std::move(currentNode);
    BookmarkManagerEvent::BookmarkManager_CurrentChanged param{m_currentNode};
    SendEvent(this, param);
    return true;
}

auto BookmarkManager::GetCurrentNode() const -> std::shared_ptr<core::BookmarkNode>
{
    return m_currentNode;
}

auto BookmarkManager::SetSelectNodes(
    std::vector<std::shared_ptr<core::BookmarkNode>> selectNodes
) -> bool
{
    if (m_selectNodes == selectNodes)
        return false;

    m_selectNodes = std::move(selectNodes);
    BookmarkManagerEvent::BookmarkManager_SelectChanged param{m_selectNodes};
    SendEvent(this, param);
    return true;
}

auto BookmarkManager::GetSelectNodes() const
    -> const std::vector<std::shared_ptr<core::BookmarkNode>>&
{
    return m_selectNodes;
}

auto BookmarkManager::DeleteCurrentNode() -> bool
{
    if (!m_currentNode)
        return false;

    auto parentNode = m_currentNode->GetParent();
    if (!parentNode)
        return false;

    const bool isErased = parentNode->EraseChild(m_currentNode);
    if (isErased)
        SetCurrentNode(nullptr);

    return isErased;
}

auto BookmarkManager::DeleteSelectNodes() -> size_t
{
    size_t result = 0;
    if (m_selectNodes.empty())
        return result;

    std::vector<std::shared_ptr<core::BookmarkNode>> selectNodes;
    std::swap(m_selectNodes, selectNodes);
    {
        BookmarkManagerEvent::BookmarkManager_SelectChanged param{m_selectNodes};
        SendEvent(this, param);
    }

    for (const auto& selectNode : selectNodes)
    {
        auto parentNode = selectNode->GetParent();
        if (parentNode)
        {
            if (parentNode->EraseChild(selectNode))
            {
                ++result;
            }
        }
    }

    return result;
}

void BookmarkManager::CallReceiveEvent(BookmarkManagerEvent& receiver) const
{
    receiver.ReceiveEvent(
        BookmarkManagerEvent::BookmarkManager_CurrentChanged{m_currentNode}
    );
    receiver.ReceiveEvent(
        BookmarkManagerEvent::BookmarkManager_SelectChanged{m_selectNodes}
    );
}

} // namespace ctrl
