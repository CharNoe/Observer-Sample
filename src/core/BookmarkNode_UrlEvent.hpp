#pragma once

#include <QString>

namespace core
{

struct BookmarkNode_UrlEventParam
{
    struct UrlChanged
    {
        QString url;
    };
};

struct BookmarkNode_UrlEvent
{
    friend class EventAccess;

    virtual void ReceiveEvent(const BookmarkNode_UrlEventParam::UrlChanged& param)
    {
    }
};

} // namespace core
