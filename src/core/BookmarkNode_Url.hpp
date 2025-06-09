#pragma once

#include "core/BookmarkNode.hpp"

namespace core {

class BookmarkNode_Url final
    : public BookmarkNode
    , public std::enable_shared_from_this<BookmarkNode_Url>
{
public:
    BookmarkNode_Url(QString name, QString url);

private:
    std::weak_ptr<BookmarkNode> m_parent;
    QString m_name;
    QString m_url;

    // BookmarkNode interface
private:
    bool SetNameImpl(const QString &name) override;
    QString GetNameImpl() const override;
    bool InsertChildImpl(std::shared_ptr<BookmarkNode> child, size_t index) override;
    std::shared_ptr<BookmarkNode> EraseChildImpl(size_t index) override;
    size_t GetChildrenSizeImpl() const override;
    void SetParentImpl(std::shared_ptr<BookmarkNode> parent) override;
    std::shared_ptr<BookmarkNode> GetParentImpl() const override;
    std::shared_ptr<BookmarkNode> SharedFromThisImpl() override;
    std::shared_ptr<BookmarkNode> GetChildImpl(size_t index) const override;
};

} // namespace core
