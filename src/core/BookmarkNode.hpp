#pragma once

#include "EventSender.hpp"
#include "core/BookmarkNodeEvent.hpp"

namespace core {

class BookmarkNode
{
public:
    BookmarkNode();
    virtual ~BookmarkNode();

    auto SetName(const QString &name) -> bool;
    auto GetName() const -> QString;

    auto InsertChild(std::shared_ptr<BookmarkNode> child, std::size_t index) -> bool;
    auto EraseChild(std::size_t index) -> std::shared_ptr<BookmarkNode>;

    EventSender<BookmarkNodeEvent> eventSender;

private:
    virtual auto SetNameImpl(const QString &name) -> bool = 0;
    virtual auto GetNameImpl() const -> QString = 0;
    // virtual auto InsertChildImpl(std::shared_ptr<BookmarkNode> child, std::size_t index) -> bool = 0;
    // virtual auto EraseChildImpl(std::size_t index) -> std::shared_ptr<BookmarkNode> = 0;
};

} // namespace core
