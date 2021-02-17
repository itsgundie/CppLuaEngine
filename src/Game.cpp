#include <iostream>
#include "Constants.h"
#include "Game.h"

Game::Game()
{
	this->isRunning = false;
}

Game::~Game()
{}

bool Game::IsRunning() const
{
	return this->isRunning;
}

float projectilePosX = 0.0f;
float projectilePosY = 0.0f;
float projectileVelX = 5.0f;
float projectileVelY = 5.0f;

void Game::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "SDL INIT FAILED :( !" << std::endl;
		return;
	}
	// SDL_DisplayMode displayMode;
	// SDL_GetCurrentDisplayMode(0, &displayMode);
	// windowWidth = displayMode.w;
	// windowHeight = displayMode.h;
	windowWidth = GAME_RENDERING_WIDTH;
	windowHeight = GAME_RENDERING_HEIGHT;
	window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
	if (!window)
	{
		std::cerr << "SDL WINDOW CREATION FAILED :( !" << std::endl;
		return;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		std::cerr << "SDL RENDERER CREATION FAILED :( !" << std::endl;
		return;
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	isRunning = true;
	return;
}

void Game::Run()
{
	while(this->IsRunning())
	{
			this->ProcessInput();
			this->Update();
			this->Render();
	}
}

void Game::ProcessInput()
{
	SDL_Event sdlEvent;
	while(SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
			case SDL_QUIT:
			{
				isRunning = false;
				break;
			}
			case SDL_KEYDOWN:
			{
				if(sdlEvent.key.keysym.sym == SDLK_ESCAPE)
				{
					isRunning = false;
				}
				break;
			}
			default:
				break;
		}
	}	
}

void Game::Update()
{
	float deltaTime = 0.1f;
	//float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
	projectilePosX += projectileVelX * deltaTime;
	projectilePosY += projectileVelY * deltaTime;
}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 42, 42, 42, 255);
	SDL_RenderClear(renderer);

	SDL_Surface *surface = IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect dstRect = {10, 10, 32, 32};
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	SDL_DestroyTexture(texture);

	// SDL_Rect projectile
	// {
	// 	(int32_t)projectilePosX, (int32_t)projectilePosY, 10, 10
	// };

	// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// SDL_RenderFillRect(renderer, &projectile);
	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}