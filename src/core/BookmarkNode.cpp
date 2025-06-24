#include "BookmarkNode.hpp"

namespace core {

BookmarkNode::BookmarkNode() {}

BookmarkNode::~BookmarkNode() {}

auto BookmarkNode::SharedFromThis() -> std::shared_ptr<BookmarkNode>
{
    return SharedFromThisImpl();
}

BookmarkKind BookmarkNode::GetKind() const
{
    return GetKindImpl();
}

bool BookmarkNode::SetName(const QString &name)
{
    const bool isChanged = SetNameImpl(name);
    if (isChanged) {
        BookmarkNodeEvent::BookmarkNode_NameChanged param{name};
        BookmarkNodeEvent::BookmarkNode_NameChangedRecursive paramRecursive{
            SharedFromThis(), name
        };
        SendEvent(this, param);
        SendEventRecursive(paramRecursive);
    }
    return isChanged;
}

QString BookmarkNode::GetName() const
{
    return GetNameImpl();
}

void BookmarkNode::SetParent(std::shared_ptr<BookmarkNode> parent)
{
    SetParentImpl(std::move(parent));
}

std::shared_ptr<BookmarkNode> BookmarkNode::GetParent() const
{
    return GetParentImpl();
}

std::shared_ptr<BookmarkNode> BookmarkNode::GetChild(size_t index) const
{
    return GetChildImpl(index);
}

bool BookmarkNode::IsInsertable(const BookmarkNode& node)
{
    return IsInsertableImpl(node);
}

bool BookmarkNode::InsertChild(std::shared_ptr<BookmarkNode> child, size_t index)
{
    const bool isChanged = InsertChildImpl(child, index);
    if (isChanged) {
        child->SetParent(SharedFromThis());
        BookmarkNodeEvent::BookmarkNode_ChildInserted param{child, index};
        BookmarkNodeEvent::BookmarkNode_ChildInsertedRecursive paramRecursive{
            SharedFromThis(), child, index
        };
        SendEvent(this, param);
        SendEventRecursive(paramRecursive);
    }
    return isChanged;
}

bool BookmarkNode::PushChild(std::shared_ptr<BookmarkNode> child)
{
    return InsertChild(std::move(child), GetChildrenSize());
}

std::shared_ptr<BookmarkNode> BookmarkNode::EraseChild(size_t index)
{
    std::shared_ptr<BookmarkNode> child = EraseChildImpl(index);
    if (child) {
        child->SetParent(nullptr);
        BookmarkNodeEvent::BookmarkNode_ChildErased param{child, index};
        BookmarkNodeEvent::BookmarkNode_ChildErasedRecursive paramRecursive{
            SharedFromThis(), child, index
        };
        SendEvent(this, param);
        SendEventRecursive(paramRecursive);
    }
    return child;
}

bool BookmarkNode::EraseChild(std::shared_ptr<BookmarkNode> child)
{
    const auto childrenSize = GetChildrenSize();
    for (size_t i = 0; i < childrenSize; ++i)
    {
        if (GetChild(i) == child)
        {
            return static_cast<bool>(EraseChild(i));
        }
    }

    return false;
}

auto BookmarkNode::GetChildrenSize() const -> size_t
{
    return GetChildrenSizeImpl();
}

void BookmarkNode::Accept(BookmarkNodeVisitor& visitor)
{
    AcceptImpl(visitor);
}

template <class EventParam>
void BookmarkNode::SendEventRecursive(const EventParam& param)
{
    std::shared_ptr<BookmarkNode> sendNode = SharedFromThis();
    while (sendNode)
    {
        SendEvent(sendNode.get(), param);
        sendNode = sendNode->GetParent();
    }
}

} // namespace core
