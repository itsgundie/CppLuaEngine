#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <stdint.h>

struct HealthComponent
{
    int32_t healthPercentage;

    HealthComponent(int32_t healthPercentage = 0)
    {
        this->healthPercentage = healthPercentage;
    }
};

#endif // !HEALTHCOMPONENT_H