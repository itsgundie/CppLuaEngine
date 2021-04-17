#ifndef PROJECTILEEMITTERSYSTEM_H
#define PROJECTILEEMITTERSYSTEM_H

#include "ECS.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "SpriteComponent.h"
#include "BoxColliderComponent.h"
#include "ProjectileEmitterComponent.h"
#include "ProjectileComponent.h"
#include "CameraFollowComponent.h"


class ProjectileEmitterSystem: public System
{
public:
    ProjectileEmitterSystem()
    {
        RequireComponent<ProjectileEmitterComponent>();
        RequireComponent<TransformComponent>();
    };

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
    {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitterSystem::OnKeyPressed);
    }

    void OnKeyPressed(KeyPressedEvent& event)
    {
        if (event.symbol == SDLK_SPACE)
        {
        	Logger::Log("SPACE PRESSED O~O");
	        for (auto entity: GetSystemEntities())
	        {
	            if (entity.HasTag("player"))
	            {
                    const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
                    const auto transform = entity.GetComponent<TransformComponent>();
                    const auto rigidBody = entity.GetComponent<RigidBodyComponent>();

                    glm::vec2 projectilePosition = transform.position;
                    if (entity.HasComponent<SpriteComponent>())
                    {
                        auto sprite = entity.GetComponent<SpriteComponent>();
                        projectilePosition.x += (transform.scale.x * sprite.width / 2);
                        projectilePosition.y += (transform.scale.y * sprite.height / 2);
                    }

                    glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
                    int32_t directionX = 0;
                    int32_t directionY = 0;
                    (rigidBody.velocity.x > 0) ? directionX = 1 : 0;
                    (rigidBody.velocity.x < 0) ? directionX = -1 : 0;
                    (rigidBody.velocity.y > 0) ? directionY = 1 : 0;
                    (rigidBody.velocity.y < 0) ? directionY = -1 : 0;

                    projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
                    projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;

                    Entity projectile = entity.registry->CreateEntity();
                    projectile.Group("projectile");
                    projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                    projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
                    projectile.AddComponent<SpriteComponent>("bullet-img", 4, 4, 4);
                    projectile.AddComponent<BoxColliderComponent>(4, 4);
                    projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);

	            }
	        }
        }
    }


    void Update(std::unique_ptr<Registry>& registry)
    {
        for(auto entity: GetSystemEntities())
        {
            auto& projectile = entity.GetComponent<ProjectileEmitterComponent>();
            const auto transform = entity.GetComponent<TransformComponent>();

            if (projectile.projectileRepeatFrequency == 0)
                continue;

            if (SDL_GetTicks() - projectile.lastEmissionTime > projectile.projectileRepeatFrequency)
            {
                glm::vec2 projectilePosition = transform.position;
                if(entity.HasComponent<SpriteComponent>())
                {
                    const auto sprite = entity.GetComponent<SpriteComponent>();
                    projectilePosition.x += (transform.scale.x * sprite.width / 2);
                    projectilePosition.y += (transform.scale.y * sprite.height / 2);
                }
                Entity newProjectile = registry->CreateEntity();
                newProjectile.Group("projectile");
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