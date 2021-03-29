#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "glm/glm.hpp"
#include "imgui.h"
#include "sol/sol.hpp"

#include "Logger.h"
#include "ECS.h"
#include "AssetManager.h"

#define GAME_TITLE "THE GAME"

const uint32_t FPS = 60;
const uint32_t MS_PER_FRAME = 1000 / FPS;

class Game
{
    private:
        bool isRunning;
        bool isDebug;
        SDL_Window *window;
        SDL_Renderer *renderer;

        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetManager> assetManager;

        uint32_t msSincePrevFrame = MS_PER_FRAME;
    public:
        uint32_t windowWidth;
        uint32_t windowHeight;
        Game();
        ~Game();
        bool IsRunning() const;
        void Init();
        void Setup();
        void LoadLevel(int32_t level);
        void Run();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
};

#endif