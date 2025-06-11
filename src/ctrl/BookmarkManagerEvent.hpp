#pragma once

#include <memory>

namespace core
{
class BookmarkNode;
} // namespace core

namespace ctrl
{

struct BookmarkManagerEventParam
{
    struct CurrentChanged
    {
        std::shared_ptr<core::BookmarkNode> currentNode;
    };
};

class BookmarkManagerEvent
{
    friend class EventAccess;

    virtual void ReceiveEvent(const BookmarkManagerEventParam::CurrentChanged& param)
    {
    }
};

} // namespace ctrl
