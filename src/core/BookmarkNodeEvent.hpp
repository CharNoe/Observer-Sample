#pragma once

#include <QString>

namespace core
{

class BookmarkNode;

struct BookmarkNodeEventParam
{
    struct NameChanged
    {
        QString name;
    };
    struct NameChangedRecursive
    {
        std::shared_ptr<BookmarkNode> node;
        QString name;
    };
    struct ChildInserted
    {
        std::shared_ptr<BookmarkNode> child;
        size_t index;
    };
    struct ChildInsertedRecursive
    {
        std::shared_ptr<BookmarkNode> node;
        std::shared_ptr<BookmarkNode> child;
        size_t index;
    };
    struct ChildErased
    {
        std::shared_ptr<BookmarkNode> child;
        size_t index;
    };
    struct ChildErasedRecursive
    {
        std::shared_ptr<BookmarkNode> node;
        std::shared_ptr<BookmarkNode> child;
        size_t index;
    };
};

class BookmarkNodeEvent
{
    friend class BookmarkNode;
    friend class EventAccess;

    virtual void ReceiveEvent(const BookmarkNodeEventParam::NameChanged& param)
    {
    }
    virtual void ReceiveEvent(const BookmarkNodeEventParam::NameChangedRecursive& param)
    {
    }
    virtual void ReceiveEvent(const BookmarkNodeEventParam::ChildInserted& param)
    {
    }
    virtual void ReceiveEvent(const BookmarkNodeEventParam::ChildInsertedRecursive& param)
    {
    }
    virtual void ReceiveEvent(const BookmarkNodeEventParam::ChildErased& param)
    {
    }
    virtual void ReceiveEvent(const BookmarkNodeEventParam::ChildErasedRecursive& param)
    {
    }

public:
    virtual ~BookmarkNodeEvent() = default;
};

} // namespace core
