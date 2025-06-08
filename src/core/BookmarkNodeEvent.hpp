#pragma once

#include <QString>

namespace core {

class BookmarkNodeEvent
{
    friend class BookmarkNode;
    virtual void BookmarkNode_NameChanged(const QString &name) {}
};

} // namespace core
