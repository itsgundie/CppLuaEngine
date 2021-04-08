#ifndef PROJECTILEEMITTERSYSTEM_H
#define PROJECTILEEMITTERSYSTEM_H

#include "ECS.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "SpriteComponent.h"
#include "BoxColliderComponent.h"
#include "ProjectileEmitterComponent.h"
#include "ProjectileComponent.h"

class ProjectileEmitterSystem: public System
{
public:
    ProjectileEmitterSystem()
    {
        RequireComponent<ProjectileEmitterComponent>();
        RequireComponent<TransformComponent>();
    };
    void Update(std::unique_ptr<Registry>& registry)
    {
        for(auto entity: GetSystemEntities())
        {
            auto& projectile = entity.GetComponent<ProjectileEmitterComponent>();
            const auto transform = entity.GetComponent<TransformComponent>();

            if (SDL_GetTicks() - projectile.lastEmissionTime > projectile.projectileRepeatFrequency)
            {
                glm::vec2 projectilePosition = transform.position;
                if(entity.HasComponent<SpriteComponent>())
                {
                    const auto sprite = entity.GetComponent<SpriteComponent>();
                    projectilePosition.x = (transform.scale.x * sprite.width / 2);
                    projectilePosition.y = (transform.scale.y * sprite.height / 2);
                }
                Entity newProjectile = registry->CreateEntity();
                newProjectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                newProjectile.AddComponent<RigidBodyComponent>(projectile.projectileVelocity);
                newProjectile.AddComponent<SpriteComponent>("bullet-img", 4, 4, 99);
                newProjectile.AddComponent<BoxColliderComponent>(4, 4);
                newProjectile.AddComponent<ProjectileComponent>(projectile.isFriendly, projectile.hitPercentDamage, projectile.projectileDuration);

                projectile.lastEmissionTime = SDL_GetTicks();
            }
        }
    }
};

#endif