#pragma once

class EventAccess
{
    template <class Event>
    friend class EventSender;

    template <class Event, class EventParam>
    static auto GetReceiveEventPtr() -> void (Event::*)(const EventParam&)
    {
        return &Event::ReceiveEvent;
    }
};
