#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include "ECS.h"
#include "Event.h"
#include "SDL.h"

class KeyPressedEvent: public Event
{
    public:
        SDL_KeyCode symbol;
        KeyPressedEvent(SDL_KeyCode symbol): symbol(symbol) {}
};

#endif