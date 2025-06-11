#include "BookmarkNode_Url.hpp"

namespace core {

BookmarkNode_Url::BookmarkNode_Url(QString name, QString url)
    : m_name{std::move(name)}
    , m_url{std::move(url)}
{}

bool BookmarkNode_Url::SetNameImpl(const QString &name)
{
    if (name == m_name)
        return false;
    m_name = name;
    return true;
}

QString BookmarkNode_Url::GetNameImpl() const
{
    return m_name;
}

bool BookmarkNode_Url::InsertChildImpl(std::shared_ptr<BookmarkNode> child, size_t index)
{
    return false;
}

std::shared_ptr<BookmarkNode> BookmarkNode_Url::EraseChildImpl(size_t index)
{
    return nullptr;
}

size_t BookmarkNode_Url::GetChildrenSizeImpl() const
{
    return 0;
}

void BookmarkNode_Url::SetParentImpl(std::shared_ptr<BookmarkNode> parent)
{
    m_parent = parent;
}

std::shared_ptr<BookmarkNode> BookmarkNode_Url::GetParentImpl() const
{
    return m_parent.lock();
}

std::shared_ptr<BookmarkNode> BookmarkNode_Url::SharedFromThisImpl()
{
    return shared_from_this();
}

std::shared_ptr<BookmarkNode> BookmarkNode_Url::GetChildImpl(size_t index) const
{
    return nullptr;
}

bool BookmarkNode_Url::IsInsertableImpl(const BookmarkNode& node)
{
    return false;
}

} // namespace core
