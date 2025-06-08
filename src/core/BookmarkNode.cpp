#include "BookmarkNode.hpp"

namespace core {

BookmarkNode::BookmarkNode() {}

BookmarkNode::~BookmarkNode() {}

bool BookmarkNode::SetName(const QString &name)
{
    const bool isChanged = SetNameImpl(name);
    if (isChanged)
        eventSender(&BookmarkNodeEvent::BookmarkNode_NameChanged, name);
    return isChanged;
}

QString BookmarkNode::GetName() const
{
    return GetNameImpl();
}

} // namespace core
