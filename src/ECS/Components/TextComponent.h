#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <string>

#include "glm/glm.hpp"
#include "SDL.h"
#include "SDL_ttf.h"

struct TextComponent
{
    glm::vec2 position;
    std::string text;
    std::string assetId;
    SDL_Color color;
    bool isFixed;
    
    TextComponent(glm::vec2 position = glm::vec2(0), std::string text = "", std::string assetId = "", const SDL_Color& color = {0}, bool isFixed = true)
    {
        this->position = position;
        this->text = text;
        this->assetId = assetId;
        this->color = color;
        this->isFixed = isFixed;
    }
};


#endif