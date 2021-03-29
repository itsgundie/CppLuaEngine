#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "ECS.h"
#include "SpriteComponent.h"
#include "AnimationComponent.h"


class AnimationSystem: public System
{
private:
    /* data */
public:
    AnimationSystem()
    {
        RequireComponent<SpriteComponent>();
        RequireComponent<AnimationComponent>();
    };
    ~AnimationSystem() = default;

    void Update()
    {
        for(auto entity: GetSystemEntities())
        {
            auto& animation = entity.GetComponent<AnimationComponent>();
            auto& sprite = entity.GetComponent<SpriteComponent>();

            animation.currentFrame = (int32_t)((SDL_GetTicks() - animation.startTime) 
                * animation.frameSpeedRate / 1000.0) % animation.numFrames;
            sprite.srcRect.x = animation.currentFrame * sprite.width;
        }
    }

};

#endif