#pragma once

#include "core/BookmarkNode.hpp"

namespace core {

class BookmarkNode_Url final : public BookmarkNode
{
public:
    BookmarkNode_Url(QString name, QString url);

private:
    QString m_name;
    QString m_url;

    // BookmarkNode interface
private:
    bool SetNameImpl(const QString &name) override;
    QString GetNameImpl() const override;
};

} // namespace core
