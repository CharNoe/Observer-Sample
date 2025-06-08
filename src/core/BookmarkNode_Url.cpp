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

} // namespace core
