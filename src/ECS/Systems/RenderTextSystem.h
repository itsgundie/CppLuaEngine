#ifndef RENDERTEXTSYSTEM_H
#define RENDERTEXTSYSTEM_H

#include "ECS.h"
#include "SDL.h"
#include "AssetManager.h"
#include "TextComponent.h"

class RenderTextSystem: public System
{
    public:
        RenderTextSystem()
        {
            RequireComponent<TextComponent>();
        };

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, const SDL_Rect& camera)
        {
            for (auto entity: GetSystemEntities())
            {
                const auto text = entity.GetComponent<TextComponent>();

                SDL_Surface* surfaceFont = TTF_RenderText_Blended(
                    assetManager->GetFont(text.assetId), 
                    text.text.c_str(), text.color);
                SDL_Texture* textureFont = SDL_CreateTextureFromSurface(renderer, surfaceFont);
                SDL_FreeSurface(surfaceFont);

                int32_t widthText = 0;
                int32_t heightText = 0;

                SDL_QueryTexture(textureFont, NULL, NULL, &widthText, &heightText);

                SDL_Rect dstRect = {
                    static_cast<int32_t>(text.position.x - (text.isFixed ? 0 : camera.x)),
                    static_cast<int32_t>(text.position.y - (text.isFixed ? 0 : camera.y)),
                    widthText, heightText
                };
                SDL_RenderCopy(renderer, textureFont, NULL, &dstRect);
                SDL_DestroyTexture(textureFont);
            }
        }        
};


#endif