#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "ECS.h"
#include "Event.h"

class CollisionEvent: public Event
{
public:
    Entity a;
    Entity b;
    CollisionEvent(Entity a, Entity b): a(a), b(b) {}
};

#endif