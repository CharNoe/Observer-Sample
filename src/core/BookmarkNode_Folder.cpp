#include "BookmarkNode_Folder.hpp"

#include "core/BookmarkNodeVisitor.hpp"

namespace core {

BookmarkNode_Folder::BookmarkNode_Folder(QString name)
    : m_name{std::move(name)}
{}

auto BookmarkNode_Folder::SetNameImpl(const QString& name) -> bool
{
    if (name == m_name)
        return false;
    m_name = name;
    return true;
}

auto BookmarkNode_Folder::GetNameImpl() const -> QString
{
    return m_name;
}

void BookmarkNode_Folder::SetParentImpl(std::shared_ptr<BookmarkNode> parent)
{
    m_parent = parent;
}

auto BookmarkNode_Folder::GetParentImpl() const -> std::shared_ptr<BookmarkNode>
{
    return m_parent.lock();
}

auto BookmarkNode_Folder::InsertChildImpl(
    std::shared_ptr<BookmarkNode> child, size_t index
) -> bool
{
    if (index > m_children.size())
        return false;

    m_children.emplace(std::next(m_children.begin(), index), std::move(child));
    return true;
}

auto BookmarkNode_Folder::EraseChildImpl(size_t index) -> std::shared_ptr<BookmarkNode>
{
    if (index >= m_children.size())
        return nullptr;

    std::shared_ptr<BookmarkNode> child = std::move(m_children[index]);
    m_children.erase(std::next(m_children.begin(), index));
    return child;
}

auto BookmarkNode_Folder::GetChildrenSizeImpl() const -> size_t
{
    return m_children.size();
}

auto BookmarkNode_Folder::SharedFromThisImpl() -> std::shared_ptr<BookmarkNode>
{
    return shared_from_this();
}

auto BookmarkNode_Folder::GetChildImpl(size_t index) const
    -> std::shared_ptr<BookmarkNode>
{
    return (index < m_children.size()) ? m_children[index] : nullptr;
}

auto BookmarkNode_Folder::IsInsertableImpl(const BookmarkNode& node) -> bool
{
    return true;
}

auto BookmarkNode_Folder::GetKindImpl() const -> BookmarkKind
{
    return BookmarkKind::Folder;
}

void BookmarkNode_Folder::AcceptImpl(BookmarkNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace core
