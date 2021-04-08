#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include <stdint.h>
#include "SDL.h"

struct ProjectileComponent
{
    bool isFriendly;
    int32_t hitPercentage;
    int32_t duration;
    int32_t startTime;

    ProjectileComponent(bool isFriendly = false, int32_t hitPercentage = 0, int32_t duration = 0)
    {
        this->isFriendly = isFriendly;
        this->hitPercentage = hitPercentage;
        this->duration = duration;
        this->startTime = SDL_GetTicks();
    }
};


#endif // !PROJECTILECOMPONENT_H