#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "glm/glm.hpp"
#include "imgui.h"
#include "sol/sol.hpp"

#define GAME_TITLE "THE GAME"

class Game
{
    private:
        bool isRunning;
        SDL_Window *window;
        SDL_Renderer *renderer;
    public:
        uint32_t windowWidth;
        uint32_t windowHeight;
        Game();
        ~Game();
        bool IsRunning() const;
        void Init();
        void Run();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
};

#endif