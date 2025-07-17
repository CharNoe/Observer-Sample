#include "BookmarkNode_Root.hpp"

#include "core/BookmarkNodeVisitor.hpp"

namespace core {

BookmarkNode_Root::BookmarkNode_Root() {}

auto BookmarkNode_Root::SetNameImpl(const QString& name) -> bool
{
    return false;
}

auto BookmarkNode_Root::GetNameImpl() const -> QString
{
    return {};
}

auto BookmarkNode_Root::InsertChildImpl(std::shared_ptr<BookmarkNode> child, size_t index)
    -> bool
{
    if (index > m_children.size())
        return false;

    m_children.emplace(std::next(m_children.begin(), index), std::move(child));
    return true;
}

auto BookmarkNode_Root::EraseChildImpl(size_t index) -> std::shared_ptr<BookmarkNode>
{
    if (index >= m_children.size())
        return nullptr;

    std::shared_ptr<BookmarkNode> child = std::move(m_children[index]);
    m_children.erase(std::next(m_children.begin(), index));
    return child;
}

auto BookmarkNode_Root::GetChildrenSizeImpl() const -> size_t
{
    return m_children.size();
}

void BookmarkNode_Root::SetParentImpl(std::shared_ptr<BookmarkNode> parent) {}

auto BookmarkNode_Root::GetParentImpl() const -> std::shared_ptr<BookmarkNode>
{
    return nullptr;
}

auto BookmarkNode_Root::SharedFromThisImpl() -> std::shared_ptr<BookmarkNode>
{
    return shared_from_this();
}

auto BookmarkNode_Root::GetChildImpl(size_t index) const -> std::shared_ptr<BookmarkNode>
{
    return (index < m_children.size()) ? m_children[index] : nullptr;
}

auto BookmarkNode_Root::IsInsertableImpl(const BookmarkNode& node) -> bool
{
    return true;
}

auto BookmarkNode_Root::GetKindImpl() const -> BookmarkKind
{
    return BookmarkKind::Root;
}

void BookmarkNode_Root::AcceptImpl(BookmarkNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace core
