#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H

#include "glm/glm.hpp"
#include "SDL.h"

struct ProjectileEmitterComponent
{
    glm::vec2   projectileVelocity;
    int32_t     projectileRepeatFrequency;
    int32_t     projectileDuration;
    int32_t     hitPercentDamage;
    int32_t     lastEmissionTime;
    bool        isFriendly;

    ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(0), 
                                int32_t projectileRepeatFrequency = 0,
                                int32_t projectileDuration = 1000,
                                int32_t hitPercentDamage = 1,
                                bool isFriendly = false)
    {
        this->projectileVelocity = projectileVelocity;
        this->projectileRepeatFrequency = projectileRepeatFrequency;
        this->projectileDuration = projectileDuration;
        this->hitPercentDamage = hitPercentDamage;
        this->isFriendly = isFriendly;
        this->lastEmissionTime = SDL_GetTicks();
    }
};


#endif