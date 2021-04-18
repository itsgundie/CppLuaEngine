#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "glm/glm.hpp"
#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_impl_sdl.h"
#include "sol.hpp"

#include "Logger.h"
#include "ECS.h"
#include "AssetManager.h"
#include "EventBus.h"

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
        SDL_Rect camera;

        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetManager> assetManager;
        std::unique_ptr<EventBus> eventBus;

        uint32_t msSincePrevFrame = MS_PER_FRAME;
    public:
        static uint32_t windowWidth;
        static uint32_t windowHeight;
        static uint32_t displayWidth;
        static uint32_t displayHeight;
        static uint32_t mapWidth;
        static uint32_t mapHeight;
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