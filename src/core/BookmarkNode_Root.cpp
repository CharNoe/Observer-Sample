#include "BookmarkNode_Root.hpp"

namespace core {

BookmarkNode_Root::BookmarkNode_Root() {}

bool BookmarkNode_Root::SetNameImpl(const QString &name)
{
    return false;
}

QString BookmarkNode_Root::GetNameImpl() const
{
    return {};
}

bool BookmarkNode_Root::InsertChildImpl(std::shared_ptr<BookmarkNode> child, size_t index)
{
    if (index > m_children.size())
        return false;

    m_children.emplace(std::next(m_children.begin(), index), std::move(child));
    return true;
}

std::shared_ptr<BookmarkNode> BookmarkNode_Root::EraseChildImpl(size_t index)
{
    if (index >= m_children.size())
        return nullptr;

    std::shared_ptr<BookmarkNode> child = std::move(m_children[index]);
    m_children.erase(std::next(m_children.begin(), index));
    return child;
}

size_t BookmarkNode_Root::GetChildrenSizeImpl() const
{
    return m_children.size();
}

void BookmarkNode_Root::SetParentImpl(std::shared_ptr<BookmarkNode> parent) {}

std::shared_ptr<BookmarkNode> BookmarkNode_Root::GetParentImpl() const
{
    return nullptr;
}

std::shared_ptr<BookmarkNode> BookmarkNode_Root::SharedFromThisImpl()
{
    return shared_from_this();
}

std::shared_ptr<BookmarkNode> BookmarkNode_Root::GetChildImpl(size_t index) const
{
    return (index < m_children.size()) ? m_children[index] : nullptr;
}

} // namespace core
