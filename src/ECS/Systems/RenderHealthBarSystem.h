#ifndef RENDERHEALTHBARSYSTEM_H
#define RENDERHEALTHBARSYSTEM_H

#include "SDL.h"
#include "ECS.h"
#include "AssetManager.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "HealthComponent.h"

class RenderHealthBarSystem: public System
{

    public:
        RenderHealthBarSystem()
        {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
            RequireComponent<HealthComponent>();
        };
        
        void Update(SDL_Renderer* renderer, const std::unique_ptr<AssetManager>& assetManager, const SDL_Rect& camera)
        {
            for (auto entity: GetSystemEntities())
            {
                const auto transform = entity.GetComponent<TransformComponent>();
                const auto sprite = entity.GetComponent<SpriteComponent>();
                const auto health = entity.GetComponent<HealthComponent>();

                SDL_Color healthBarColor = {255, 255, 255};

                if (health.healthPercentage >= 0 && health.healthPercentage <= 30)
                    healthBarColor = {255, 0, 0};
                if (health.healthPercentage > 30 && health.healthPercentage <= 80)
                    healthBarColor = {255, 255, 0};
                if (health.healthPercentage > 80)
                    healthBarColor = {0, 255, 0};
            
                int32_t healthBarWidth = 45;
                int32_t healthBarHeight = 5;
                double_t healthBarPosX = (transform.position.x + 
                        (sprite.width * transform.scale.x)) - camera.x;
                double_t healthBarPosY = transform.position.y - camera.y;

                SDL_Rect healthBarRect = {
                    static_cast<int32_t>(healthBarPosX),
                    static_cast<int32_t>(healthBarPosY),
                    static_cast<int32_t>(healthBarWidth * (health.healthPercentage / 100.0f)),
                    static_cast<int32_t>(healthBarHeight)
                };

                SDL_SetRenderDrawColor(renderer, healthBarColor.r, healthBarColor.g, healthBarColor.b, 255);
                SDL_RenderFillRect(renderer, &healthBarRect);

                std::string healthBarText = std::to_string(health.healthPercentage);
                SDL_Surface* surface = TTF_RenderText_Blended(assetManager->GetFont("pico12-font"), healthBarText.c_str(), healthBarColor);
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);

                int32_t textWidth = 0;
                int32_t textHeight = 0;
                SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
                SDL_Rect healthBarTextRect = {
                    static_cast<int32_t>(healthBarPosX),
                    static_cast<int32_t>(healthBarPosY) + 10,
                    textWidth, textHeight
                };

                SDL_RenderCopy(renderer, texture, NULL, &healthBarTextRect);
                SDL_DestroyTexture(texture);
            }
        }
};


#endif