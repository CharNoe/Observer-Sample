#pragma once

#include "EventMacro.hpp"
#include <QString>

namespace core
{

class BookmarkNode_UrlEvent
{
    friend class EventAccess;

public:
    virtual ~BookmarkNode_UrlEvent() = default;

    EVENT_DEF(BookmarkNode_Url_UrlChanged)
    {
        QString url;
    };
};

} // namespace core
