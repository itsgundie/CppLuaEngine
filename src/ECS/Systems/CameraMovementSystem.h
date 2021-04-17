#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include "ECS.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "CameraFollowComponent.h"


class CameraMovementSystem: public System
{
public:
    CameraMovementSystem()
    {
        RequireComponent<CameraFollowComponent>();
        RequireComponent<TransformComponent>();
    };
    void Update(SDL_Rect& camera)
    {
        for(auto entity: GetSystemEntities())
        {
            auto transform = entity.GetComponent<TransformComponent>();

            if(transform.position.x + (camera.w / 2) < Game::mapWidth)
                camera.x = transform.position.x - (Game::windowWidth / 2);
            if(transform.position.y + (camera.h / 2) < Game::mapHeight)
                camera.y = transform.position.y - (Game::windowHeight / 2);
            // camera.x = camera.x < 0 ? 0 : camera.x;
            // camera.y = camera.y < 0 ? 0 : camera.y;
            // camera.x = camera.x > camera.w ? camera.w : camera.x;
            // camera.y = camera.y > camera.h ? camera.h : camera.y;
            camera.x = std::clamp(camera.x, 0, camera.w);
            camera.y = std::clamp(camera.y, 0, camera.h);
        }
    }
};

#endif