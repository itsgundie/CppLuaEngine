#include <iostream>
#include <fstream>
#include "Constants.h"
#include "Game.h"
#include "ECS.h"
#include "glm/glm.hpp"

#include "TransformComponent.h"
#include "MovementSystem.h"
#include "RigidBodyComponent.h"

#include "SpriteComponent.h"
#include "AnimationComponent.h"
#include "RenderSystem.h"
#include "AnimationSystem.h"

#include "BoxColliderComponent.h"
#include "CollisionSystem.h"

#include "DamageSystem.h"
#include "KeyboardControlSystem.h"


Game::Game()
{
	this->isRunning = false;
	this->isDebug = false;
	// registry = Registry();
	registry = std::make_unique<Registry>();
	assetManager = std::make_unique<AssetManager>();
	eventBus = std::make_unique<EventBus>();
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


void Game::LoadLevel(int32_t level)
{


	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();
	registry->AddSystem<DamageSystem>();
	registry->AddSystem<KeyboardControlSystem>();

	// Adding assets to asset manager
	assetManager->AddTexture(renderer, "tank_panther_right", "./assets/images/tank-panther-right.png");
	assetManager->AddTexture(renderer, "tank_panther_left", "./assets/images/tank-panther-left.png");
	assetManager->AddTexture(renderer, "tank_panther_up", "./assets/images/tank-panther-up.png");
	assetManager->AddTexture(renderer, "tank_panther_down", "./assets/images/tank-panther-down.png");
	assetManager->AddTexture(renderer, "tilemap-jungle", "./assets/tilemaps/jungle.png");
	assetManager->AddTexture(renderer, "chopper-img", "./assets/images/chopper.png");
	assetManager->AddTexture(renderer, "radar-img", "./assets/images/radar.png");



	// Loading Tile Map
	int32_t tileSize = 32;
	double_t tileScale = 2.0;
	int32_t mapNumColumns = 25;
	int32_t mapNumRows = 20;

	std::fstream jungleMap;
	jungleMap.open("./assets/tilemaps/jungle.map");

	for(int32_t y = 0; y < mapNumRows; y++)
	{
		for(int32_t x = 0; x < mapNumColumns; x++)
		{
			char charInTile;
			
			jungleMap.get(charInTile);
			int32_t srcRectY = std::atoi(&charInTile) * tileSize;

			jungleMap.get(charInTile);
			int32_t srcRectX = std::atoi(&charInTile) * tileSize;
			jungleMap.ignore();

			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize),
				y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0f);

			tile.AddComponent<SpriteComponent>("tilemap-jungle", tileSize, tileSize, 0, srcRectX, srcRectY);
		}
	}

	Entity chopper = registry->CreateEntity();
	chopper.AddComponent<TransformComponent>(glm::vec2(500.0f, 500.0f), glm::vec2(4.0f, 4.0f), 0.0f);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0f, 0.0f));
	chopper.AddComponent<SpriteComponent>("chopper-img", 32, 32, 3);
	chopper.AddComponent<AnimationComponent>(2, 8, true);

	Entity radar = registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(1500.0f, 50.0f), glm::vec2(1.0f, 1.0f), 0.0f);
	radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0f, 0.0f));
	radar.AddComponent<SpriteComponent>("radar-img", 64, 64, 99);
	radar.AddComponent<AnimationComponent>(8, 4, true);

	// Creating Entity And Adding to it Components
	Entity tank = registry->CreateEntity();
	// Entity anotherOne = registry->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(11.0f, 11.0f), glm::vec2(1.0f, 1.0f), 33.0f);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(2222.0f, 1299.0f));
	tank.AddComponent<SpriteComponent>("tank_panther_right", 32, 32, 1);
	tank.AddComponent<BoxColliderComponent>(32, 32);


	Entity antitank = registry->CreateEntity();
	// Entity anotherOne = registry->CreateEntity();
	antitank.AddComponent<TransformComponent>(glm::vec2(1800.0f, 1000.0f), glm::vec2(5.0f, 5.0f), 21.0f);
	antitank.AddComponent<RigidBodyComponent>(glm::vec2(-2222.0f, -1299.0f));
	antitank.AddComponent<SpriteComponent>("tank_panther_left", 32, 32, 2);
	// bounding box not considering scale of object, need to think about it
	antitank.AddComponent<BoxColliderComponent>(32 * 5, 32 * 5);

}


void Game::Setup()
{
	LoadLevel(1);


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
				if(sdlEvent.key.keysym.sym == SDLK_d)
				{
					isDebug= !isDebug;
				}
				eventBus->EmitEvent<KeyPressedEvent>(static_cast<SDL_KeyCode>(sdlEvent.key.keysym.sym));
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

	eventBus->Reset();
	registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
	registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);

	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CollisionSystem>().Update(eventBus);

	// At the End of Frame add/remove entities in que to proceed
	registry->Update();


}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 42, 42, 42, 255);
	SDL_RenderClear(renderer);

	registry->GetSystem<RenderSystem>().Update(renderer, assetManager);
	if(isDebug)
		registry->GetSystem<CollisionSystem>().RenderBoxCollision(renderer);
	
	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	Logger::Err("It's Dead |-_-\"|");
}