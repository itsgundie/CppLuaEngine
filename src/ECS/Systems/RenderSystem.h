#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "SDL.h"

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"


class RenderSystem: public System
{
    public:
        RenderSystem()
        {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(SDL_Renderer* renderer)
        {
            for (Entity entity: GetSystemEntities())
            {
                const TransformComponent transform = entity.GetComponent<TransformComponent>();
                const SpriteComponent sprite = entity.GetComponent<SpriteComponent>();

                SDL_Rect rectangleToDraw =
                {
                    static_cast<int32_t>(transform.position.x),
                    static_cast<int32_t>(transform.position.y),
                    sprite.width, sprite.height
                };

                SDL_SetRenderDrawColor(renderer, 31, 63, 127, 255);
                SDL_RenderFillRect(renderer, &rectangleToDraw);

            }
        }
};


#endif