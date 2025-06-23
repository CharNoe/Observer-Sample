#pragma once

#include "EventAccess.hpp"
#include <sigslot/adapter/qt.hpp>
#include <sigslot/signal.hpp>

template <class Event>
class EventSender
{
    template <class EventType, class DerivType>
    friend auto Connect(
        EventSender<EventType>& sender, std::shared_ptr<DerivType> receiver
    ) -> sigslot::connection;

    template <class EventType, class DerivType>
    friend auto ConnectQt(EventSender<EventType>& sender, DerivType* receiver)
        -> sigslot::connection;

    template <class EventType, class EventParam>
    friend void SendEvent(EventSender<EventType>* sender, const EventParam& param);

private:
    using UpdateFunc = void (*)(Event&, const void*);
    struct Slot
    {
        Event* event;
        void operator()(UpdateFunc func, const void* param)
        {
            func(*event, param);
        }
    };

    sigslot::signal_st<UpdateFunc, const void*> m_sig;
};

template <class EventType, class DerivType>
auto Connect(EventSender<EventType>& sender, std::shared_ptr<DerivType> receiver)
    -> sigslot::connection
{
    typename EventSender<EventType>::Slot slot{receiver.get()};
    return sender.m_sig.connect(slot, std::move(receiver));
}

template <class EventType, class DerivType>
auto ConnectQt(EventSender<EventType>& sender, DerivType* receiver) -> sigslot::connection
{
    typename EventSender<EventType>::Slot slot{receiver};
    return sender.m_sig.connect(slot, static_cast<QObject*>(receiver));
}

template <class EventType, class EventParam>
void SendEvent(EventSender<EventType>* sender, const EventParam& param)
{
    auto func = [](EventType& event, const void* param)
    {
        auto func = EventAccess::GetReceiveEventPtr<EventType, EventParam>();
        (event.*func)(*static_cast<const EventParam*>(param));
    };
    sender->m_sig(func, &param);
}
