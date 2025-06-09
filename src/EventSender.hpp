#pragma once

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

    template <class... FuncArgTypes, class... ArgTypes>
    void operator()(void (Event::*func)(FuncArgTypes...), const ArgTypes&... args)
    {
        auto lambda = [&... args = args, func](Event& event)
        {
            (event.*func)(args...);
        };
        EventCallerOf caller{std::move(lambda)};
        m_sig(caller);
    }

private:
    struct EventCaller
    {
        virtual void Call(Event& event) = 0;
    };
    template <class Lambda>
    struct EventCallerOf : EventCaller
    {
        EventCallerOf(Lambda&& lambda)
            : m_lambda{std::move(lambda)}
        {
        }
        void Call(Event& event) override
        {
            m_lambda(event);
        }

    private:
        Lambda m_lambda;
    };
    struct Slot
    {
        Event* event;
        void operator()(EventCaller& caller)
        {
            caller.Call(*event);
        }
    };

    sigslot::signal_st<EventCaller&> m_sig;
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
