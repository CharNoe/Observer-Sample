#pragma once

#include <sigslot/signal.hpp>

class EventBase;

namespace detail
{

void SetDefaultConnection(EventBase& event, sigslot::connection connection);
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
        EventBase& event, sigslot::connection connection
    );
    friend void detail::DisconnectDefault(EventBase& event);

    sigslot::connection m_defaultConnection;
};
