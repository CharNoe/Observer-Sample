#pragma once

#include "EventSender.hpp"
#include "core/BookmarkNodeEvent.hpp"

namespace core {

class BookmarkNode
{
public:
    BookmarkNode();
    virtual ~BookmarkNode();

    auto SharedFromThis() -> std::shared_ptr<BookmarkNode>;

    auto SetName(const QString &name) -> bool;
    auto GetName() const -> QString;

    void SetParent(std::shared_ptr<BookmarkNode> parent);
    auto GetParent() const -> std::shared_ptr<BookmarkNode>;
    auto GetChild(size_t index) const -> std::shared_ptr<BookmarkNode>;
    auto InsertChild(std::shared_ptr<BookmarkNode> child, size_t index) -> bool;
    auto PushChild(std::shared_ptr<BookmarkNode> child) -> bool;
    auto EraseChild(size_t index) -> std::shared_ptr<BookmarkNode>;
    auto GetChildrenSize() const -> size_t;

    EventSender<BookmarkNodeEvent> eventSender;

private:
    virtual auto SharedFromThisImpl() -> std::shared_ptr<BookmarkNode> = 0;
    virtual auto SetNameImpl(const QString &name) -> bool = 0;
    virtual auto GetNameImpl() const -> QString = 0;
    virtual void SetParentImpl(std::shared_ptr<BookmarkNode> parent) = 0;
    virtual auto GetParentImpl() const -> std::shared_ptr<BookmarkNode> = 0;
    virtual auto GetChildImpl(size_t index) const -> std::shared_ptr<BookmarkNode> = 0;
    virtual auto InsertChildImpl(std::shared_ptr<BookmarkNode> child, size_t index) -> bool = 0;
    virtual auto EraseChildImpl(size_t index) -> std::shared_ptr<BookmarkNode> = 0;
    virtual auto GetChildrenSizeImpl() const -> size_t = 0;

    template<class FuncType, class... ArgTypes>
    void SendEventRecursive(FuncType func, const ArgTypes &...args);
};

} // namespace core
