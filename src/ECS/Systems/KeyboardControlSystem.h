#ifndef KEYBOARDCONTROLLSYSTEM_H
#define KEYBOARDCONTROLLSYSTEM_H

#include "ECS.h"
#include "EventBus.h"
#include "KeyPressedEvent.h"
#include "RigidBodyComponent.h"
#include "SpriteComponent.h"

class KeyboardControlSystem: public System
{
    public:
        KeyboardControlSystem()
        {};
    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
    {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }
    void OnKeyPressed(KeyPressedEvent& event)
    {
        std::string keyCode = std::to_string(event.symbol);
        std::string keySymbol(1, event.symbol);
        Logger::Log("Key pressed event emitted: [" + keyCode + "] " + keySymbol);
    }
};



#endif