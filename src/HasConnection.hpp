#pragma once

#include <sigslot/signal.hpp>

template <class Event>
class HasConnection
{
    sigslot::connection m_connection;
};
