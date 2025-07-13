#pragma once

#include <boost/signals2.hpp>
#include <sigslot/signal.hpp>

// using EventConnection = sigslot::connection;
using EventConnection = boost::signals2::connection;
