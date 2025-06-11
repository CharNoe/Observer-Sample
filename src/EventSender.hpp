#pragma once

#include "EventAccess.hpp"
#include <sigslot/adapter/qt.hpp>
#include <sigslot/signal.hpp>

template <class Event>
class EventSender final
{
public:
    auto Connect(std::shared_ptr<Event> receiver) -> sigslot::connection;
    auto Disconnect(std::shared_ptr<Event> receiver) -> size_t;
    template <class Type>
    auto ConnectQt(Type* receiver) -> sigslot::connection;
    auto DisconnectQt(QObject* receiver) -> size_t;

    template <class EventParam>
    void operator()(const EventParam& param);

private:
    struct EventCaller;
    template <class EventParam>
    struct EventCallerOf;
    struct Slot;

    sigslot::signal_st<EventCaller&> m_sig;
};

template <class Event>
struct EventSender<Event>::EventCaller
{
    virtual void Call(Event& event) = 0;
};

template <class Event>
template <class EventParam>
struct EventSender<Event>::EventCallerOf : EventSender<Event>::EventCaller
{
    EventCallerOf(const EventParam& param)
        : m_param{param}
    {
    }
    void Call(Event& event) override
    {
        auto func = EventAccess::GetReceiveEventPtr<Event, EventParam>();
        (event.*func)(m_param);
    }

private:
    EventParam m_param;
};

template <class Event>
struct EventSender<Event>::Slot
{
    Event* event;
    void operator()(EventCaller& caller)
    {
        caller.Call(*event);
    }
};

template <class Event>
inline auto EventSender<Event>::Connect(std::shared_ptr<Event> receiver)
    -> sigslot::connection
{
    Slot slot{receiver.get()};
    return m_sig.connect(slot, std::move(receiver));
}

template <class Event>
inline auto EventSender<Event>::Disconnect(std::shared_ptr<Event> receiver) -> size_t
{
    return m_sig.disconnect(receiver);
}

template <class Event>
template <class Type>
inline sigslot::connection EventSender<Event>::ConnectQt(Type* receiver)
{
    Slot slot{receiver};
    return m_sig.connect(slot, static_cast<QObject*>(receiver));
}

template <class Event>
inline size_t EventSender<Event>::DisconnectQt(QObject* receiver)
{
    return m_sig.disconnect(receiver);
}

template <class Event>
template <class EventParam>
inline void EventSender<Event>::operator()(const EventParam& param)
{
    EventCallerOf<EventParam> caller{param};
    m_sig(caller);
}
