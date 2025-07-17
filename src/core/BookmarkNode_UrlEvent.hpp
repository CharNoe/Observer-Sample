#pragma once

#include "EventBase.hpp"
#include "EventMacro.hpp"
#include <QString>

namespace core
{

class BookmarkNode_UrlEvent : public EventBase
{
public:
    EVENT_DEF(BookmarkNode_Url_UrlChanged)
    {
        QString url;
    };
};

} // namespace core
