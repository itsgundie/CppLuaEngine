#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <stdint.h>

#include "SDL.h"

struct AnimationComponent
{
    int32_t numFrames;
    int32_t currentFrame;
    int32_t frameSpeedRate;
    int32_t startTime;
    bool isLooped;

    AnimationComponent(int32_t numFrames = 1, int32_t frameSpeedRate = 1, bool isLooped = true)
    {
        this->numFrames = numFrames;
        this->currentFrame = 1;
        this->frameSpeedRate = frameSpeedRate;
        this->isLooped = isLooped;
        this->startTime = SDL_GetTicks();
    }

};
#endif