#pragma once

#include "EventConnection.hpp"

class EventBase;

namespace detail
{

void SetDefaultConnection(EventBase& event, EventConnection connection);
void DisconnectDefault(EventBase& event);

} // namespace detail

class EventBase
{
    friend void detail::SetDefaultConnection(
        EventBase& event, EventConnection connection
    );
    friend void detail::DisconnectDefault(EventBase& event);

protected:
    EventBase() = default;

public:
    virtual ~EventBase() = default;

private:
    EventConnection m_defaultConnection;
};
