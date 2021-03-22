#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <stdint.h>

struct SpriteComponent
{
    int32_t width;
    int32_t height;

    // If method called without arguments, component will get standart values
    SpriteComponent(int32_t width = 0, int32_t height = 0)
    {
        this->width = width;
        this->height = height;
    }
};

#endif