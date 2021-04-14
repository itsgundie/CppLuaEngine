#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "Logger.h"
#include "ECS.h"
#include "BoxColliderComponent.h"
#include "EventBus.h"
#include "CollisionEvent.h"
#include "ProjectileComponent.h"
#include "HealthComponent.h"

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
        Entity a = event.a;
        Entity b = event.b;

        Logger::Log("Entity " + std::to_string(a.GetId()) + " is collided with Entity " + std::to_string(b.GetId()));

        if (a.BelongsToGroup("projectile") && b.HasTag("player"))
        {
            OnProjectileHitsPlayer(a, b);
        }
        if (b.BelongsToGroup("projectile") && a.HasTag("player"))
        {
            OnProjectileHitsPlayer(b, a);
        }
        if (a.BelongsToGroup("projectile") && b.BelongsToGroup("enemy"))
        {
            OnProjectileHitsEnemy(a, b);
        }
        if (b.BelongsToGroup("projectile") && a.BelongsToGroup("enemy"))
        {
            OnProjectileHitsEnemy(b, a);
        }

    }

    void    OnProjectileHitsPlayer(Entity projectile, Entity player)
    {
        auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if (!projectileComponent.isFriendly)
        {
            auto& health = player.GetComponent<HealthComponent>();
            health.healthPercentage -= projectileComponent.hitPercentage;
            Logger::Log("Player Health is " + std::to_string(health.healthPercentage) + " !!!");
            if (health.healthPercentage <= 0)
                player.Kill();
        }
        projectile.Kill();
    }

    void    OnProjectileHitsEnemy(Entity projectile, Entity enemy)
    {
        auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if (projectileComponent.isFriendly)
        {
            auto& health = enemy.GetComponent<HealthComponent>();
            health.healthPercentage -= projectileComponent.hitPercentage;
            Logger::Log("Enemy Health is " + std::to_string(health.healthPercentage) + " ^-^");
            if (health.healthPercentage <= 0)
                enemy.Kill();
        }
        projectile.Kill();
    }
    

    void    Update()
    {

    }
};

#endif