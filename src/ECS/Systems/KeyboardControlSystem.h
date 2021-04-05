#ifndef KEYBOARDCONTROLLSYSTEM_H
#define KEYBOARDCONTROLLSYSTEM_H

#include "ECS.h"
#include "EventBus.h"
#include "KeyPressedEvent.h"
#include "RigidBodyComponent.h"
#include "SpriteComponent.h"
#include "KeyboardControlComponent.h"

class KeyboardControlSystem: public System
{
    public:
        KeyboardControlSystem()
        {
            RequireComponent<KeyboardControlComponent>();
            RequireComponent<SpriteComponent>();
            RequireComponent<RigidBodyComponent>();
        };
    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
    {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }
    void OnKeyPressed(KeyPressedEvent& event)
    {
        for(auto entity: GetSystemEntities())
        {
            const auto keyboardControl = entity.GetComponent<KeyboardControlComponent>();
            auto& sprite = entity.GetComponent<SpriteComponent>();
            auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

            switch (event.symbol)
            {
            case (SDLK_w):
                rigidBody.velocity = keyboardControl.upVelocity;
                sprite.srcRect.y = sprite.height * 0;
                break;
            case (SDLK_d):
                rigidBody.velocity = keyboardControl.rightVelocity;
                sprite.srcRect.y = sprite.height * 1;
                break;
            case (SDLK_s):
                rigidBody.velocity = keyboardControl.downVelocity;
                sprite.srcRect.y = sprite.height * 2;
                break;
            case (SDLK_a):
                rigidBody.velocity = keyboardControl.leftVelocity;
                sprite.srcRect.y = sprite.height * 3;
                break;
            default:
                break;
            }
            // Logger::Log("RigidBody velocity NOW is : [" + std::to_string(rigidBody.velocity.x) + "," + std::to_string(rigidBody.velocity.y) + "]");
        }

        std::string keyCode = std::to_string(event.symbol);
        std::string keySymbol(1, event.symbol);
        Logger::Log("Key pressed event emitted: [" + keyCode + "] " + keySymbol);
    }
};



#endif