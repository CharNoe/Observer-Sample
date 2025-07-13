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
protected:
    EventBase() = default;

public:
    virtual ~EventBase() = default;

private:
    friend void detail::SetDefaultConnection(
        EventBase& event, EventConnection connection
    );
    friend void detail::DisconnectDefault(EventBase& event);

    EventConnection m_defaultConnection;
};
