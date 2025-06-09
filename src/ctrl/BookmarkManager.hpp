#pragma once

#include <memory>

namespace core
{
class BookmarkNode_Root;
class BookmarkNode;
} // namespace core

namespace ctrl
{

class BookmarkManager final
{
public:
    BookmarkManager();

    auto GetRootBookmark() const -> std::shared_ptr<core::BookmarkNode_Root>;
    auto GetRootBookmarkBase() const -> std::shared_ptr<core::BookmarkNode>;

private:
    const std::shared_ptr<core::BookmarkNode_Root> m_rootBookmark;
};

} // namespace ctrl
