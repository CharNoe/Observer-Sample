#pragma once

#include "EventAccess.hpp"
#include "EventBase.hpp"
#include "EventConnection.hpp"
#include <boost/signals2.hpp>
#include <qdebug.h>
#include <sigslot/adapter/qt.hpp>
#include <sigslot/signal.hpp>

template <class Event>
class EventSender
{
    template <class EventType, class DerivType>
    friend auto Connect(
        EventSender<EventType>& sender, std::shared_ptr<DerivType> receiver
    ) -> EventConnection;

    template <class EventType, class DerivType>
    friend void AutoConnect(
        const std::shared_ptr<EventSender<EventType>>& sender,
        std::shared_ptr<DerivType> receiver
    );

    template <class EventType, class DerivType>
    friend auto ConnectQt(EventSender<EventType>& sender, DerivType* receiver)
        -> EventConnection;

    template <class EventType, class DerivType>
    friend void AutoConnectQt(
        const std::shared_ptr<EventSender<EventType>>& sender, DerivType* receiver
    );

    template <class EventType, class EventParam>
    friend void SendEvent(EventSender<EventType>* sender, const EventParam& param);

private:
    using UpdateFunc = void (*)(Event&, const void*);
    struct Slot final
    {
        Event* event;
        void operator()(UpdateFunc func, const void* param)
        {
            func(*event, param);
        }
    };

    // sigslot::signal_st<UpdateFunc, const void*> m_sig;
    boost::signals2::signal<void(UpdateFunc, const void*)> m_sig;
};

template <class EventType, class DerivType>
auto Connect(EventSender<EventType>& sender, std::shared_ptr<DerivType> receiver)
    -> EventConnection
{
    typename EventSender<EventType>::Slot slot{receiver.get()};
    return sender.m_sig.connect(slot, std::move(receiver));
}

template <class EventType, class DerivType>
void AutoConnect(
    const std::shared_ptr<EventSender<EventType>>& sender,
    std::shared_ptr<DerivType> receiver
)
{
    static_assert(std::is_base_of_v<EventBase, EventType>);
    if (sender)
    {
        typename EventSender<EventType>::Slot slot{receiver.get()};
        auto connection = sender->m_sig.connect(slot, std::move(receiver));
        detail::SetDefaultConnection(
            *std::static_pointer_cast<EventType>(receiver), std::move(connection)
        );
    }
    else
    {
        detail::DisconnectDefault(*std::static_pointer_cast<EventType>(receiver));
    }
}

template <class EventType, class DerivType>
auto ConnectQt(EventSender<EventType>& sender, DerivType* receiver) -> EventConnection
{
    typename EventSender<EventType>::Slot slot{receiver};
    // return sender.m_sig.connect(slot, static_cast<QObject*>(receiver));
    return sender.m_sig.connect(slot);
}

template <class EventType, class DerivType>
void AutoConnectQt(
    const std::shared_ptr<EventSender<EventType>>& sender, DerivType* receiver
)
{
    static_assert(std::is_base_of_v<EventBase, EventType>);
    if (sender)
    {
        typename EventSender<EventType>::Slot slot{receiver};
        // auto connection = sender->m_sig.connect(slot, static_cast<QObject*>(receiver));
        auto connection = sender->m_sig.connect(slot);
        detail::SetDefaultConnection(
            *static_cast<EventType*>(receiver), std::move(connection)
        );
    }
    else
    {
        detail::DisconnectDefault(*static_cast<EventType*>(receiver));
    }
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
