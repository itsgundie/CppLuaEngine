#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "ECS.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "EventBus.h"
#include "CollisionEvent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "SpriteComponent.h"

class MovementSystem: public System
{
    private:
        /* data */
    public:
        MovementSystem()
        {
            RequireComponent<TransformComponent>();
            RequireComponent<RigidBodyComponent>();
            // RequireComponent<>();
        };

        void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus)
        {
            eventBus->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollisionEvent);
        }

        void OnCollisionEvent(CollisionEvent& event)
        {
            Entity a = event.a;
            Entity b = event.b;

            Logger::Log("Entity " + std::to_string(a.GetId()) + " is collided with Entity " + std::to_string(b.GetId()));

            if (a.BelongsToGroup("enemy") && b.BelongsToGroup("obstacle"))
                OnEnemyHitObstacle(a, b);
            if (b.BelongsToGroup("enemy") && a.BelongsToGroup("obstacle"))
                OnEnemyHitObstacle(b, a);
        }

        void OnEnemyHitObstacle(Entity enemy, Entity obstacle)
        {
            if (enemy.HasComponent<RigidBodyComponent>()
                && enemy.HasComponent<SpriteComponent>())
            {
                auto& enemyRigidBody = enemy.GetComponent<RigidBodyComponent>();
                auto& enemySprite = enemy.GetComponent<SpriteComponent>();

                if (enemyRigidBody.velocity.x != 0)
                {
                    enemyRigidBody.velocity.x *= -1.0f;
                    enemySprite.flip = (enemySprite.flip == SDL_FLIP_NONE)
                         ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
                }
                if (enemyRigidBody.velocity.y != 0)
                {
                    enemyRigidBody.velocity.y *= -1.0f;
                    enemySprite.flip = (enemySprite.flip == SDL_FLIP_NONE)
                         ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
                }    
            }
        }
        void Update(const double_t deltaTime)
        {
            for (Entity entity:GetSystemEntities())
            {
                TransformComponent& transform = entity.GetComponent<TransformComponent>();
                const RigidBodyComponent rigidBody = entity.GetComponent<RigidBodyComponent>();

                transform.position.x += rigidBody.velocity.x * deltaTime;
                transform.position.y += rigidBody.velocity.y * deltaTime;

                if (entity.HasTag("player"))
                {
                    int32_t paddingTop = 10;
                    int32_t paddingRight = 120;
                    int32_t paddingBottom = 120;
                    int32_t paddingLeft = 10;

                    transform.position.x = transform.position.x < paddingLeft
                                        ? paddingLeft : transform.position.x;
                    transform.position.x = transform.position.x > 
                                            Game::mapWidth - paddingRight 
                                            ? Game::mapWidth - paddingRight
                                            : transform.position.x;
                    transform.position.y = transform.position.y < paddingTop
                                        ? paddingTop : transform.position.y;
                    transform.position.y = transform.position.y > 
                                            Game::mapHeight - paddingBottom 
                                            ? Game::mapHeight - paddingBottom
                                            : transform.position.y;
                }

                int32_t margin = 128;

                bool isEntityOutBounds = (
                transform.position.x < -margin ||
                transform.position.x > Game::mapWidth + margin ||
                transform.position.y < -margin ||
                transform.position.y > Game::mapHeight + margin
                );

                if (isEntityOutBounds && !entity.HasTag("player"))
                {
                    entity.Kill();
                    Logger::Log("Entity ID = " + std::to_string(entity.GetId()) + " killed by OutOfBounds");
                }
                
                // Logger::Log("Entity ID = " + std::to_string(entity.GetId()) 
                //     + " position is now at (" + std::to_string(transform.position.x) + ", "
                //     + std::to_string(transform.position.y) + ")");
            }

    }

};


#endif