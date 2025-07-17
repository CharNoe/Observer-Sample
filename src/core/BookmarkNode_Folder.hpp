#pragma once

#include "core/BookmarkNode.hpp"
#include <vector>

namespace core {

class BookmarkNode_Folder final
    : public BookmarkNode
    , public std::enable_shared_from_this<BookmarkNode_Folder>
{
public:
    BookmarkNode_Folder(QString name);

private:
    std::weak_ptr<BookmarkNode> m_parent;
    QString m_name;
    std::vector<std::shared_ptr<BookmarkNode>> m_children;

    // BookmarkNode interface
private:
    auto SetNameImpl(const QString& name) -> bool override;
    auto GetNameImpl() const -> QString override;
    void SetParentImpl(std::shared_ptr<BookmarkNode> parent) override;
    auto GetParentImpl() const -> std::shared_ptr<BookmarkNode> override;
    auto InsertChildImpl(std::shared_ptr<BookmarkNode> child, size_t index)
        -> bool override;
    auto EraseChildImpl(size_t index) -> std::shared_ptr<BookmarkNode> override;
    auto GetChildrenSizeImpl() const -> size_t override;
    auto SharedFromThisImpl() -> std::shared_ptr<BookmarkNode> override;
    auto GetChildImpl(size_t index) const -> std::shared_ptr<BookmarkNode> override;
    auto IsInsertableImpl(const BookmarkNode& node) -> bool override;
    auto GetKindImpl() const -> BookmarkKind override;
    void AcceptImpl(BookmarkNodeVisitor& visitor) override;
};

} // namespace core
