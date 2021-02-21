#include <iostream>
#include "Constants.h"
#include "Game.h"
#include "ECS.h"
#include "glm/glm.hpp"

Game::Game()
{
	this->isRunning = false;
	Logger::Log("Game Constructor Call");
}

Game::~Game()
{
	Logger::Log("Game Destructor Call");
}

bool Game::IsRunning() const
{
	return this->isRunning;
}

void Game::Init()
{	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::Err("SDL INIT FAILED :( !");
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
		Logger::Err("SDL RENDERER CREATION FAILED :( !");
		return;
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	isRunning = true;
	return;
}

// float projectilePosX = 0.0f;
// float projectilePosY = 0.0f;
// float projectileVelX = 5.0f;
// float projectileVelY = 5.0f;
glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup()
{
	// playerPosition = glm::vec2(1.0, 20.0);
	// playerVelocity = glm::vec2(200.0, 50.0);
	
	// Entity some registry.CreateEntity();
	// some.AddComponent<TransformComponent>();.
	
	Logger::Log("Game Setup Call");
}

void Game::Run()
{
	this->Init();
	this->Setup();
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
	//double_t deltaTime = (SDL_GetTicks() - msSincePrevFrame) / 1000.0;
	// Capping FPS here
	uint32_t timeToWait = MS_PER_FRAME - (SDL_GetTicks() - msSincePrevFrame);
	if (timeToWait > 0 && timeToWait <= MS_PER_FRAME)
		SDL_Delay(timeToWait);
	
	msSincePrevFrame = SDL_GetTicks();

	// playerPosition.x += playerVelocity.x * deltaTime;
	// playerPosition.y += playerVelocity.y * deltaTime;
	// MovementSystem.Update();
	// CollisionSystem.Update();
	//DamageSystem.Update();


}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 42, 42, 42, 255);
	SDL_RenderClear(renderer);

	// SDL_Surface *surface = IMG_Load("./assets/images/tank-tiger-right.png");
	// SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	// SDL_FreeSurface(surface);

	// SDL_Rect dstRect = {static_cast<int>(playerPosition.x), 
	// 					static_cast<int>(playerPosition.y), 
	// 					32, 32};
	// SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	// SDL_DestroyTexture(texture);

	// SDL_Rect projectile
	// {
	// 	(int32_t)projectilePosX, (int32_t)projectilePosY, 10, 10
	// };

	// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// SDL_RenderFillRect(renderer, &projectile);
	
	// SystemReder
	
	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	Logger::Err("It's Dead |-_-\"|");
}