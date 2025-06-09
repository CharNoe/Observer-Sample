#pragma once

#include <memory>

namespace ctrl
{

class BookmarkManager;

class System final
{
public:
    System();

    auto GetBookmarkManager() const -> std::shared_ptr<BookmarkManager>;

private:
    const std::shared_ptr<BookmarkManager> m_bookmarkManager;
};

} // namespace ctrl
