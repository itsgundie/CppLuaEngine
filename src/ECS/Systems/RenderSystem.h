#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <vector>

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

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera)
        {
            // Temporal struct to hold both components needed for sorting
            struct RenderebleEntity
            {
                TransformComponent transform;
                SpriteComponent sprite;
            };
            
            std::vector<RenderebleEntity> renderebleEntities;

            // Gather entities to sort and render in one vector structure
            for(auto entity: GetSystemEntities())
            {
                RenderebleEntity renderebleEntity;
                renderebleEntity.sprite = entity.GetComponent<SpriteComponent>();
                renderebleEntity.transform = entity.GetComponent<TransformComponent>();

                renderebleEntities.emplace_back(renderebleEntity);
            }

            // Sort all entities of system to render by z-index
            // Using lambda function to check zIndex for sorting condition
            sort(renderebleEntities.begin(), renderebleEntities.end(), 
            [](const RenderebleEntity& a, RenderebleEntity& b)
            {
                return a.sprite.zIndex < b.sprite.zIndex;
            });

            // Render sorted in vector entities
            for (auto entity: renderebleEntities)
            {
                const TransformComponent transform = entity.transform;
                const SpriteComponent sprite = entity.sprite;

                SDL_Rect srcRect = sprite.srcRect;
                
                SDL_Rect dstRect = {
                    static_cast<int32_t>(transform.position.x - camera.x),
                    static_cast<int32_t>(transform.position.y - camera.y),
                    static_cast<int32_t>(sprite.width * transform.scale.x),
                    static_cast<int32_t>(sprite.height * transform.scale.y)
                    };

                SDL_RenderCopyEx(renderer, assetManager->GetTexture(sprite.assetId),
                     &srcRect, &dstRect, transform.rotation, NULL, SDL_FLIP_NONE);
            }
        }
};


#endif