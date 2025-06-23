#pragma once

#include "core/BookmarkNode.hpp"
#include <vector>

namespace core {

class BookmarkNode_Root final
    : public BookmarkNode
    , public std::enable_shared_from_this<BookmarkNode_Root>
{
public:
    BookmarkNode_Root();

private:
    std::vector<std::shared_ptr<BookmarkNode>> m_children;

    // BookmarkNode interface
private:
    auto SetNameImpl(const QString& name) -> bool override;
    auto GetNameImpl() const -> QString override;
    auto InsertChildImpl(std::shared_ptr<BookmarkNode> child, size_t index)
        -> bool override;
    auto EraseChildImpl(size_t index) -> std::shared_ptr<BookmarkNode> override;
    auto GetChildrenSizeImpl() const -> size_t override;
    void SetParentImpl(std::shared_ptr<BookmarkNode> parent) override;
    auto GetParentImpl() const -> std::shared_ptr<BookmarkNode> override;
    auto SharedFromThisImpl() -> std::shared_ptr<BookmarkNode> override;
    auto GetChildImpl(size_t index) const -> std::shared_ptr<BookmarkNode> override;
    bool IsInsertableImpl(const BookmarkNode& node) override;
    BookmarkKind GetKindImpl() const override;
    void AcceptImpl(BookmarkNodeVisitor& visitor) override;
};

} // namespace core
