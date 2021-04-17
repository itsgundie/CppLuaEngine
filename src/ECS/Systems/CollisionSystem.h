#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "SDL.h"
#include "ECS.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"
#include "CollisionEvent.h"

class CollisionSystem: public System
{
    public:
        CollisionSystem()
        {
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        };

        bool CheckCollisionAABB(double_t aX, double_t aY, double_t aWidth, double_t aHeight, double_t bX, double bY, double_t bWidth, double_t bHeight)
        {
            return(
                aX < bX + bWidth &&
                aX + aWidth > bX &&
                aY < bY + bHeight &&
                aY + aHeight > bY
            );
        };

        void Update(std::unique_ptr<EventBus>& eventBus)
        {
            auto entities = GetSystemEntities();
            for(auto i = entities.begin(); i != entities.end(); i++)
            {
                Entity entityA = *i;
                TransformComponent aTransform = entityA.GetComponent<TransformComponent>();
                BoxColliderComponent aCollider = entityA.GetComponent<BoxColliderComponent>();
                for(auto j = i + 1; j != entities.end(); j++)
                {
                    Entity entityB = *j;
                    TransformComponent bTransform = entityB.GetComponent<TransformComponent>();
                    BoxColliderComponent bCollider = entityB.GetComponent<BoxColliderComponent>();

                    bool isCollided = CheckCollisionAABB(
                        aTransform.position.x + aCollider.offset.x, 
                        aTransform.position.y + aCollider.offset.y,
                        aCollider.width, aCollider.height,
                        bTransform.position.x + bCollider.offset.x,
                        bTransform.position.y + bCollider.offset.y,
                        bCollider.width, bCollider.height
                    );

                    if(isCollided)
                    {
                        eventBus->EmitEvent<CollisionEvent>(entityA, entityB);
                    }
                }
            }
        }

        void RenderBoxCollision(SDL_Renderer* renderer, SDL_Rect& camera)
        {
            for(auto entity: GetSystemEntities())
            {
                const TransformComponent transform = entity.GetComponent<TransformComponent>();
                const BoxColliderComponent collider = entity.GetComponent<BoxColliderComponent>();

                SDL_Rect colliderRect = {
                    static_cast<int>(transform.position.x + collider.offset.x - camera.x),
                    static_cast<int>(transform.position.y + collider.offset.y - camera.y),
                    static_cast<int>(collider.width),
                    static_cast<int>(collider.height)
                };
                // Should choose different color if collision happened
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &colliderRect);
            }
        }
};


#endif