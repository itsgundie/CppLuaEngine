#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#define GAME_TITLE "THE GAME"

class Game
{
    private:
        bool isRunning;
        SDL_Window *window;
        SDL_Renderer *renderer;
    public:
        Game();
        ~Game();
        bool IsRunning() const;
        void Init(uint32_t width, uint32_t height);
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
};

#endif