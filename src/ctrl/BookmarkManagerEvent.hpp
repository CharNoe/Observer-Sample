#pragma once

#include "EventBase.hpp"
#include "EventMacro.hpp"
#include <memory>
#include <vector>

namespace core
{
class BookmarkNode;
} // namespace core

namespace ctrl
{

class BookmarkManagerEvent : public EventBase
{
    friend class EventAccess;
    friend class BookmarkManager;

public:
    virtual ~BookmarkManagerEvent() = default;

    EVENT_DEF(BookmarkManager_CurrentChanged)
    {
        std::shared_ptr<core::BookmarkNode> currentNode;
    };
    EVENT_DEF(BookmarkManager_SelectChanged)
    {
        const std::vector<std::shared_ptr<core::BookmarkNode>>& selectNodes;
    };
};

} // namespace ctrl
