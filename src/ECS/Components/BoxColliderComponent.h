#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <stdint.h>

#include "glm/glm.hpp"


struct BoxColliderComponent
{
    int32_t width;
    int32_t height;
    glm::vec2 offset;

    BoxColliderComponent(int32_t width = 0, int32_t height = 0, glm::vec2 offset = glm::vec2(0,0))
    {
        this->width = width;
        this->height = height;
        this->offset = offset;
    }
};



#endif