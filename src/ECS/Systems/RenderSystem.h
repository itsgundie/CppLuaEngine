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

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager)
        {
            for (Entity entity: GetSystemEntities())
            {
                const TransformComponent transform = entity.GetComponent<TransformComponent>();
                const SpriteComponent sprite = entity.GetComponent<SpriteComponent>();

                // Set rectangle size for source
                SDL_Rect srcRect = sprite.srcRect;
                // Set rectangle size for output
                SDL_Rect dstRect = {
                    static_cast<int32_t>(transform.position.x),
                    static_cast<int32_t>(transform.position.y),
                    static_cast<int32_t>(sprite.width * transform.scale.x),
                    static_cast<int32_t>(sprite.height * transform.scale.y)
                    };

                SDL_RenderCopyEx(renderer, assetManager->GetTexture(sprite.assetId),
                     &srcRect, &dstRect, transform.rotation, NULL, SDL_FLIP_NONE);

                // SDL_Rect rectangleToDraw =
                // {
                //     static_cast<int32_t>(transform.position.x),
                //     static_cast<int32_t>(transform.position.y),
                //     sprite.width, sprite.height
                // };

                // SDL_SetRenderDrawColor(renderer, 31, 63, 127, 255);
                // SDL_RenderFillRect(renderer, &rectangleToDraw);

            }
        }
};


#endif