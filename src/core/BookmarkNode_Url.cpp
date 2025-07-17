#include "BookmarkNode_Url.hpp"

#include "core/BookmarkNodeVisitor.hpp"

namespace core {

BookmarkNode_Url::BookmarkNode_Url(QString name, QString url)
    : m_name{std::move(name)}
    , m_url{std::move(url)}
{}

auto BookmarkNode_Url::SetUrl(QString url) -> bool
{
    if (m_url == url)
    {
        return false;
    }

    m_url = std::move(url);
    BookmarkNode_UrlEvent::BookmarkNode_Url_UrlChanged param{m_url};
    SendEvent<BookmarkNode_UrlEvent>(this, param);
    return true;
}

auto BookmarkNode_Url::GetUrl() const -> QString
{
    return m_url;
}

auto BookmarkNode_Url::SetNameImpl(const QString& name) -> bool
{
    if (name == m_name)
        return false;
    m_name = name;
    return true;
}

auto BookmarkNode_Url::GetNameImpl() const -> QString
{
    return m_name;
}

auto BookmarkNode_Url::InsertChildImpl(std::shared_ptr<BookmarkNode> child, size_t index)
    -> bool
{
    return false;
}

auto BookmarkNode_Url::EraseChildImpl(size_t index) -> std::shared_ptr<BookmarkNode>
{
    return nullptr;
}

auto BookmarkNode_Url::GetChildrenSizeImpl() const -> size_t
{
    return 0;
}

void BookmarkNode_Url::SetParentImpl(std::shared_ptr<BookmarkNode> parent)
{
    m_parent = parent;
}

auto BookmarkNode_Url::GetParentImpl() const -> std::shared_ptr<BookmarkNode>
{
    return m_parent.lock();
}

auto BookmarkNode_Url::SharedFromThisImpl() -> std::shared_ptr<BookmarkNode>
{
    return shared_from_this();
}

auto BookmarkNode_Url::GetChildImpl(size_t index) const -> std::shared_ptr<BookmarkNode>
{
    return nullptr;
}

auto BookmarkNode_Url::IsInsertableImpl(const BookmarkNode& node) -> bool
{
    return false;
}

auto BookmarkNode_Url::GetKindImpl() const -> BookmarkKind
{
    return BookmarkKind::Url;
}

void BookmarkNode_Url::AcceptImpl(BookmarkNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace core
