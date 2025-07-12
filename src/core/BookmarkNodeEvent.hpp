#pragma once

#include "EventBase.hpp"
#include "EventMacro.hpp"
#include <QString>

namespace core
{

class BookmarkNode;

class BookmarkNodeEvent : public EventBase
{
    friend class EventAccess;

public:
    virtual ~BookmarkNodeEvent() = default;

    EVENT_DEF(BookmarkNode_NameChanged)
    {
        QString name;
    };
    EVENT_DEF(BookmarkNode_NameChangedRecursive)
    {
        std::shared_ptr<BookmarkNode> node;
        QString name;
    };
    EVENT_DEF(BookmarkNode_ChildInserted)
    {
        std::shared_ptr<BookmarkNode> child;
        size_t index;
    };
    EVENT_DEF(BookmarkNode_ChildInsertedRecursive)
    {
        std::shared_ptr<BookmarkNode> node;
        std::shared_ptr<BookmarkNode> child;
        size_t index;
    };
    EVENT_DEF(BookmarkNode_ChildErased)
    {
        std::shared_ptr<BookmarkNode> child;
        size_t index;
    };
    EVENT_DEF(BookmarkNode_ChildErasedRecursive)
    {
        std::shared_ptr<BookmarkNode> node;
        std::shared_ptr<BookmarkNode> child;
        size_t index;
    };
};

} // namespace core
