#include "System.hpp"

#include "ctrl/BookmarkManager.hpp"

namespace ctrl
{

System::System()
    : m_bookmarkManager{std::make_shared<BookmarkManager>()}
{
}

auto System::GetBookmarkManager() const -> std::shared_ptr<BookmarkManager>
{
    return m_bookmarkManager;
}

} // namespace ctrl
