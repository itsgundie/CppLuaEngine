#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "Logger.h"
#include "ECS.h"
#include "BoxColliderComponent.h"
#include "EventBus.h"
#include "CollisionEvent.h"

class DamageSystem: public System
{
public:
    DamageSystem()
    {
        RequireComponent<BoxColliderComponent>();
    };
    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
    {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollisionEvent);
    }
    void OnCollisionEvent(CollisionEvent& event)
    {
        event.a.Kill();
        event.b.Kill();
        Logger::Log("Entity " + std::to_string(event.a.GetId()) + " is collided with Entity " + std::to_string(event.b.GetId()));
    }

    void Update()
    {

    }
};

#endif