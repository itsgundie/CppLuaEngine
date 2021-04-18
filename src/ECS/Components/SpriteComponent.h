#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <stdint.h>
#include "SDL.h"
struct SpriteComponent
{
    int32_t width;
    int32_t height;
    int32_t zIndex;
    bool    isFixed;
    std::string assetId;
    SDL_RendererFlip flip;

    SDL_Rect srcRect;

    // If method called without arguments, component will get standart values
    SpriteComponent(std::string assetId = "", int32_t width = 0, int32_t height = 0, int32_t zIndex = 0, bool isFixed = false, int32_t srcRectX = 0, int32_t srcRectY = 0)
    {
        this->width = width;
        this->height = height;
        this->assetId = assetId;
        this->zIndex = zIndex;
        this->isFixed = isFixed;
        this->srcRect = { srcRectX, srcRectY, width, height};
        this->flip = SDL_FLIP_NONE;
    }
};

#endif