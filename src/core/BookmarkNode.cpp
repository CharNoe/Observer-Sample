#include "BookmarkNode.hpp"

namespace core {

BookmarkNode::BookmarkNode() {}

BookmarkNode::~BookmarkNode() {}

auto BookmarkNode::SharedFromThis() -> std::shared_ptr<BookmarkNode>
{
    return SharedFromThisImpl();
}

bool BookmarkNode::SetName(const QString &name)
{
    const bool isChanged = SetNameImpl(name);
    if (isChanged) {
        eventSender(&BookmarkNodeEvent::BookmarkNode_NameChanged, name);
        SendEventRecursive(&BookmarkNodeEvent::BookmarkNode_NameChangedRecursive, name);
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

bool BookmarkNode::InsertChild(std::shared_ptr<BookmarkNode> child, size_t index)
{
    const bool isChanged = InsertChildImpl(child, index);
    if (isChanged) {
        child->SetParent(SharedFromThis());
        eventSender(&BookmarkNodeEvent::BookmarkNode_ChildInserted, child, index);
        SendEventRecursive(&BookmarkNodeEvent::BookmarkNode_ChildInsertedRecursive, child, index);
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
        eventSender(&BookmarkNodeEvent::BookmarkNode_ChildErased, child, index);
        SendEventRecursive(&BookmarkNodeEvent::BookmarkNode_ChildErasedRecursive, child, index);
    }
    return child;
}

auto BookmarkNode::GetChildrenSize() const -> size_t
{
    return GetChildrenSizeImpl();
}

template<class FuncType, class... ArgTypes>
void BookmarkNode::SendEventRecursive(FuncType func, const ArgTypes &...args)
{
    std::shared_ptr<BookmarkNode> sendNode = SharedFromThis();
    const std::shared_ptr<BookmarkNode> node = sendNode;
    while (sendNode)
    {
        eventSender(func, node, args...);
        sendNode = sendNode->GetParent();
    }
}

} // namespace core
