#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "SDL.h"
#include "ECS.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"

class CollisionSystem: public System
{
    private:

        bool CheckCollisionAABB(double_t aX, double_t aY, double_t aWidth, double_t aHeight, double_t bX, double bY, double_t bWidth, double_t bHeight)
        {
            return(
                aX < bX + bWidth &&
                aX + aWidth > bX &&
                aY < bY + bHeight &&
                aY + aHeight > bY
            );
        };

    public:
        CollisionSystem()
        {
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        };

        void Update()
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
                        entityA.Kill();
                        entityB.Kill();
                        Logger::Log("Entity " + std::to_string(entityA.GetId()) + " is collided with Entity " + std::to_string(entityB.GetId()));
                    }
                }
            }
        }

        void RenderBoxCollision(SDL_Renderer* renderer)
        {
            for(auto entity: GetSystemEntities())
            {
                const TransformComponent transform = entity.GetComponent<TransformComponent>();
                const BoxColliderComponent collider = entity.GetComponent<BoxColliderComponent>();

                SDL_Rect colliderRect = {
                    static_cast<int>(transform.position.x + collider.offset.x),
                    static_cast<int>(transform.position.y + collider.offset.y),
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