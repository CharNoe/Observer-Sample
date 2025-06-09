#pragma once

#include <QString>

namespace core
{

class BookmarkNodeEvent
{
    friend class BookmarkNode;

    virtual void BookmarkNode_NameChanged(const QString& name)
    {
    }
    virtual void BookmarkNode_NameChangedRecursive(
        const std::shared_ptr<BookmarkNode>& node, const QString& name
    )
    {
    }
    virtual void BookmarkNode_ChildInserted(
        const std::shared_ptr<BookmarkNode>& child, size_t index
    )
    {
    }
    virtual void BookmarkNode_ChildInsertedRecursive(
        const std::shared_ptr<BookmarkNode>& node,
        const std::shared_ptr<BookmarkNode>& child,
        size_t index
    )
    {
    }
    virtual void BookmarkNode_ChildErased(
        const std::shared_ptr<BookmarkNode>& name, size_t index
    )
    {
    }
    virtual void BookmarkNode_ChildErasedRecursive(
        const std::shared_ptr<BookmarkNode>& node,
        const std::shared_ptr<BookmarkNode>& name,
        size_t index
    )
    {
    }

public:
    virtual ~BookmarkNodeEvent() = default;
};

} // namespace core
