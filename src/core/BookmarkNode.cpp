#include "BookmarkNode.hpp"

namespace core {

BookmarkNode::BookmarkNode() {}

BookmarkNode::~BookmarkNode() {}

auto BookmarkNode::SharedFromThis() -> std::shared_ptr<BookmarkNode>
{
    return SharedFromThisImpl();
}

auto BookmarkNode::GetKind() const -> BookmarkKind
{
    return GetKindImpl();
}

auto BookmarkNode::SetName(const QString& name) -> bool
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

auto BookmarkNode::GetName() const -> QString
{
    return GetNameImpl();
}

void BookmarkNode::SetParent(std::shared_ptr<BookmarkNode> parent)
{
    SetParentImpl(std::move(parent));
}

auto BookmarkNode::GetParent() const -> std::shared_ptr<BookmarkNode>
{
    return GetParentImpl();
}

auto BookmarkNode::GetChild(size_t index) const -> std::shared_ptr<BookmarkNode>
{
    return GetChildImpl(index);
}

auto BookmarkNode::IsInsertable(const BookmarkNode& node) -> bool
{
    return IsInsertableImpl(node);
}

auto BookmarkNode::InsertChild(std::shared_ptr<BookmarkNode> child, size_t index) -> bool
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

auto BookmarkNode::PushChild(std::shared_ptr<BookmarkNode> child) -> bool
{
    return InsertChild(std::move(child), GetChildrenSize());
}

auto BookmarkNode::EraseChild(size_t index) -> std::shared_ptr<BookmarkNode>
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

auto BookmarkNode::EraseChild(std::shared_ptr<BookmarkNode> child) -> bool
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
