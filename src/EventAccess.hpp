#pragma once

template <class Event>
class EventSender;

class EventAccess
{
    template <class EventType, class EventParam>
    friend void SendEvent(EventSender<EventType>* sender, const EventParam& param);

    template <class Event, class EventParam>
    static auto GetReceiveEventPtr() -> void (Event::*)(const EventParam&)
    {
        return &Event::ReceiveEvent;
    }
};
