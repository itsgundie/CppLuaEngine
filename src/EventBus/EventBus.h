#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <map>
#include <list>
#include <typeindex>

#include "Logger.h"
#include "Event.h"

class IEventCallback
{
    public:
        virtual ~IEventCallback() = default;
        void Execute(Event& event)
        {
            Call(event);
        }
    private:
        virtual void Call(Event& event) = 0;
};

template <typename TOwner, typename TEvent>
class EventCallback: public IEventCallback
{
    typedef void (TOwner::*CallbackFunction)(TEvent&);
    public:
        EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction)
        {
           this->ownerInstance = ownerInstance;
           this->callbackFunction = callbackFunction; 
        }
        virtual ~EventCallback() override = default;
    private:
        TOwner* ownerInstance;
        CallbackFunction callbackFunction;

        virtual void Call(Event& event) override
        {
            std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(event));
        }
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus
{
public:
    EventBus()
    {
        Logger::Log("EventBus constructor call");
    }
    
    ~EventBus()
    {
        Logger::Log("EventBus destructor call");
    }

    void Reset()
    {
        subscribers.clear();
    }

    // Listener subscribes to a type of event with specific func/method to call
    // eventBus->SubscribeToEvent<WeGotGhostInTheHouseEvent>(&Phone::CallGhostbusters);
    template <typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&))
    {
        if(!subscribers[typeid(TEvent)].get())
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();

        auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
        subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    // Emit event, as soon it's happened, 
    // all listener's callback funcs/methods will be executed
    // eventBus->EmitEvent<WeGotGhostInTheHouseEvent>(myAdress, ghost);
    template <typename TEvent, typename ...TArgs>
    void EmitEvent(TArgs&& ...args)
    {
        auto handlers = subscribers[typeid(TEvent)].get();
        if(handlers)
        {
            for(auto itera = handlers->begin(); itera != handlers->end(); itera++)
            {
                auto handler = itera->get();
                TEvent event(std::forward<TArgs>(args)...);
                handler->Execute(event);
            }
        }
    }


    private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;
};

#endif