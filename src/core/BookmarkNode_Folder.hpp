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
    bool SetNameImpl(const QString &name) override;
    QString GetNameImpl() const override;
    void SetParentImpl(std::shared_ptr<BookmarkNode> parent) override;
    std::shared_ptr<BookmarkNode> GetParentImpl() const override;
    bool InsertChildImpl(std::shared_ptr<BookmarkNode> child, size_t index) override;
    std::shared_ptr<BookmarkNode> EraseChildImpl(size_t index) override;
    size_t GetChildrenSizeImpl() const override;
    std::shared_ptr<BookmarkNode> SharedFromThisImpl() override;
    std::shared_ptr<BookmarkNode> GetChildImpl(size_t index) const override;
};

} // namespace core
