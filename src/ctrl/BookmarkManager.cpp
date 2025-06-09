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

std::shared_ptr<core::BookmarkNode> BookmarkManager::GetRootBookmarkBase() const
{
    return m_rootBookmark;
}

} // namespace ctrl
