#pragma once

namespace core
{

class BookmarkNode_Root;
class BookmarkNode_Folder;
class BookmarkNode_Url;

class BookmarkNodeVisitor
{
public:
    BookmarkNodeVisitor() = default;
    virtual ~BookmarkNodeVisitor() = default;

    virtual void Visit(BookmarkNode_Root& node) = 0;
    virtual void Visit(BookmarkNode_Folder& node) = 0;
    virtual void Visit(BookmarkNode_Url& node) = 0;
};

} // namespace core
