#include "EventBase.hpp"

namespace detail
{

void SetDefaultConnection(EventBase& event, sigslot::connection connection)
{
    event.m_defaultConnection.disconnect();
    event.m_defaultConnection = std::move(connection);
}

void DisconnectDefault(EventBase& event)
{
    event.m_defaultConnection.disconnect();
}

} // namespace detail
