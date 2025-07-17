#pragma once

#include "core/BookmarkNode.hpp"
#include "core/BookmarkNode_UrlEvent.hpp"

namespace core {

class BookmarkNode_Url final
    : public BookmarkNode
    , public EventSender<BookmarkNode_UrlEvent>
    , public std::enable_shared_from_this<BookmarkNode_Url>
{
public:
    BookmarkNode_Url(QString name, QString url);

    auto SetUrl(QString url) -> bool;
    auto GetUrl() const -> QString;

private:
    std::weak_ptr<BookmarkNode> m_parent;
    QString m_name;
    QString m_url;

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
    auto IsInsertableImpl(const BookmarkNode& node) -> bool override;
    auto GetKindImpl() const -> BookmarkKind override;
    void AcceptImpl(BookmarkNodeVisitor& visitor) override;
};

} // namespace core
