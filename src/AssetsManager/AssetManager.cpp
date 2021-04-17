
#include "AssetManager.h"
#include "Logger.h"

#include "SDL_image.h"

AssetManager::AssetManager()
{
    Logger::Log("AssetManager Constructor Call");
}

AssetManager::~AssetManager()
{
    ClearAssets();
    Logger::Log("AssetManager Destructor Call");
}

void AssetManager::ClearAssets()
{
    for (auto texture: textures)
        SDL_DestroyTexture(texture.second);
    textures.clear();

    for (auto font: fonts)
        TTF_CloseFont(font.second);
    fonts.clear();
}

void AssetManager::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath)
{
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    // Adding Texture to the Map
    textures.emplace(assetId, texture);
    Logger::Log("Texture Added to Asset Manager with ID = " + assetId);
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetId)
{
    return(textures[assetId]);
}

void AssetManager::AddFont(const std::string& assetId, const std::string& filePath, int32_t fontSize)
{
    fonts.emplace(assetId, TTF_OpenFont(filePath.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(const std::string& assetId)
{
    return(fonts[assetId]);
}