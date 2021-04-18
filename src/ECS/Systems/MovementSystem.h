#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "ECS.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"

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
        ~MovementSystem() = default;

    void Update(const double_t deltaTime)
    {
        for (Entity entity:GetSystemEntities())
        {
            TransformComponent& transform = entity.GetComponent<TransformComponent>();
            const RigidBodyComponent rigidBody = entity.GetComponent<RigidBodyComponent>();

            transform.position.x += rigidBody.velocity.x * deltaTime;
            transform.position.y += rigidBody.velocity.y * deltaTime;

            bool isEntityOutBounds = (
                transform.position.x < 0 ||
                transform.position.x > Game::mapWidth ||
                transform.position.y < 0 ||
                transform.position.y > Game::mapHeight
            );

            if (isEntityOutBounds && !entity.HasTag("player"))
                entity.Kill();
            // Logger::Log("Entity ID = " + std::to_string(entity.GetId()) 
            //     + " position is now at (" + std::to_string(transform.position.x) + ", "
            //     + std::to_string(transform.position.y) + ")");
        }

    }

};


#endif