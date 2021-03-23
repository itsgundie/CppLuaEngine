#include <iostream>
#include "Constants.h"
#include "Game.h"
#include "ECS.h"
#include "glm/glm.hpp"

#include "TransformComponent.h"
#include "MovementSystem.h"
#include "RigidBodyComponent.h"

#include "SpriteComponent.h"
#include "RenderSystem.h"

Game::Game()
{
	this->isRunning = false;
	// registry = Registry();
	registry = std::make_unique<Registry>();
	assetManager = std::make_unique<AssetManager>();
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

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup()
{
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();

	// Adding assets to asset manager
	assetManager->AddTexture(renderer, "tank_panther_right", "./assets/images/tank-panther-right.png");
	assetManager->AddTexture(renderer, "tank_panther_left", "./assets/images/tank-panther-left.png");
	assetManager->AddTexture(renderer, "tank_panther_up", "./assets/images/tank-panther-up.png");
	assetManager->AddTexture(renderer, "tank_panther_down", "./assets/images/tank-panther-down.png");


	// Creating Entity And Adding to it Components
	Entity tank = registry->CreateEntity();
	// Entity anotherOne = registry->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(11.0f, 11.0f), glm::vec2(1.0f, 1.0f), 33.0f);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(2000.0f, 2000.0f));

	tank.AddComponent<SpriteComponent>("tank_panther_right", 32, 32);

	Entity antitank = registry->CreateEntity();
	// Entity anotherOne = registry->CreateEntity();
	antitank.AddComponent<TransformComponent>(glm::vec2(1800.0f, 1000.0f), glm::vec2(5.0f, 5.0f), 21.0f);
	antitank.AddComponent<RigidBodyComponent>(glm::vec2(-2222.0f, -1299.0f));

	antitank.AddComponent<SpriteComponent>("tank_panther_left", 32, 32);


	// tank.RemoveComponent<TransformComponent>();

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
	double_t deltaTime = (SDL_GetTicks() - msSincePrevFrame) / 1000.0;
	// Capping FPS here
	uint32_t timeToWait = MS_PER_FRAME - (SDL_GetTicks() - msSincePrevFrame);
	if (timeToWait > 0 && timeToWait <= MS_PER_FRAME)
		SDL_Delay(timeToWait);
	
	msSincePrevFrame = SDL_GetTicks();

	registry->GetSystem<MovementSystem>().Update(deltaTime);


	// At the End of Frame add/remove entities in que to proceed
	registry->Update();


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

	registry->GetSystem<RenderSystem>().Update(renderer, assetManager);
	
	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	Logger::Err("It's Dead |-_-\"|");
}