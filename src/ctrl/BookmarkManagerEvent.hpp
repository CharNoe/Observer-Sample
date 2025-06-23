#pragma once

#include "EventMacro.hpp"
#include <memory>

namespace core
{
class BookmarkNode;
} // namespace core

namespace ctrl
{

class BookmarkManagerEvent
{
    friend class EventAccess;

public:
    virtual ~BookmarkManagerEvent() = default;

    EVENT_DEF(BookmarkManagerEvent_CurrentChanged)
    {
        std::shared_ptr<core::BookmarkNode> currentNode;
    };
};

} // namespace ctrl
