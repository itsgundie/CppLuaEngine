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

void Game::Init(uint32_t width, uint32_t height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "SDL INIT FAILED :( !" << std::endl;
		return;
	}
	window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);
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

	isRunning = true;
	return;
}

void Game::ProcessInput()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
		{
			isRunning = false;
			break;
		}
		case SDL_KEYDOWN:
		{
			if(event.key.keysym.sym == SDLK_ESCAPE)
				isRunning = false;
		}
		default:
			break;
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

	SDL_Rect projectile
	{
		(int32_t)projectilePosX, (int32_t)projectilePosY, 10, 10
	};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &projectile);
	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}