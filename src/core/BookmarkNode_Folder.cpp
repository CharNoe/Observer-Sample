#include "BookmarkNode_Folder.hpp"

namespace core {

BookmarkNode_Folder::BookmarkNode_Folder(QString name)
    : m_name{std::move(name)}
{}

bool BookmarkNode_Folder::SetNameImpl(const QString &name)
{
    if (name == m_name)
        return false;
    m_name = name;
    return true;
}

QString BookmarkNode_Folder::GetNameImpl() const
{
    return m_name;
}

void BookmarkNode_Folder::SetParentImpl(std::shared_ptr<BookmarkNode> parent)
{
    m_parent = parent;
}

std::shared_ptr<BookmarkNode> BookmarkNode_Folder::GetParentImpl() const
{
    return m_parent.lock();
}

bool BookmarkNode_Folder::InsertChildImpl(std::shared_ptr<BookmarkNode> child, size_t index)
{
    if (index > m_children.size())
        return false;

    m_children.emplace(std::next(m_children.begin(), index), std::move(child));
    return true;
}

std::shared_ptr<BookmarkNode> BookmarkNode_Folder::EraseChildImpl(size_t index)
{
    if (index >= m_children.size())
        return nullptr;

    std::shared_ptr<BookmarkNode> child = std::move(m_children[index]);
    m_children.erase(std::next(m_children.begin(), index));
    return child;
}

size_t BookmarkNode_Folder::GetChildrenSizeImpl() const
{
    return m_children.size();
}

std::shared_ptr<BookmarkNode> BookmarkNode_Folder::SharedFromThisImpl()
{
    return shared_from_this();
}

std::shared_ptr<BookmarkNode> BookmarkNode_Folder::GetChildImpl(size_t index) const
{
    return (index < m_children.size()) ? m_children[index] : nullptr;
}

} // namespace core
