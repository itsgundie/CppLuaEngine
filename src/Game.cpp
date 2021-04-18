#include <iostream>
#include <fstream>

#include "Constants.h"
#include "Game.h"
#include "ECS.h"
// #include "glm/glm.hpp"


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

#include "KeyboardControlComponent.h"

#include "CameraMovementSystem.h"
#include "CameraFollowComponent.h"

#include "ProjectileEmitterComponent.h"
#include "ProjectileEmitterSystem.h"
#include "ProjectileComponent.h"
#include "ProjectileLifeCycleSystem.h"

#include "HealthComponent.h"
#include "RenderHealthBarSystem.h"

#include "TextComponent.h"
#include "RenderTextSystem.h"

#include "RenderGuiSystem.h"

uint32_t Game::windowWidth;
uint32_t Game::windowHeight;
uint32_t Game::mapWidth;
uint32_t Game::mapHeight;


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

	if (TTF_Init() != 0)
	{
		Logger::Err("SDL_TTF INIT FAILED :(!");
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
	SDL_DisplayMode screenInfo;
	SDL_GetCurrentDisplayMode(0, &screenInfo);
	ImGui::CreateContext();
	ImGuiSDL::Initialize(renderer, screenInfo.w, screenInfo.h);
	
	camera.x = 0;
	camera.y = 0;
	camera.w = windowWidth;
	camera.h = windowHeight;

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
	registry->AddSystem<CameraMovementSystem>();
	registry->AddSystem<ProjectileEmitterSystem>();
	registry->AddSystem<ProjectileLifeCycleSystem>();
	registry->AddSystem<RenderTextSystem>();
	registry->AddSystem<RenderHealthBarSystem>();
	registry->AddSystem<RenderGuiSystem>();

	// Adding assets to asset manager
	assetManager->AddTexture(renderer, "tank_panther_right", "./assets/images/tank-panther-right.png");
	assetManager->AddTexture(renderer, "tank_panther_left", "./assets/images/tank-panther-left.png");
	assetManager->AddTexture(renderer, "tank_panther_up", "./assets/images/tank-panther-up.png");
	assetManager->AddTexture(renderer, "tank_panther_down", "./assets/images/tank-panther-down.png");
	assetManager->AddTexture(renderer, "tilemap_jungle", "./assets/tilemaps/jungle.png");
	assetManager->AddTexture(renderer, "chopper_img", "./assets/images/chopper-spritesheet.png");
	assetManager->AddTexture(renderer, "radar_img", "./assets/images/radar.png");
	assetManager->AddTexture(renderer, "bullet_img", "./assets/images/bullet.png");

	assetManager->AddFont("charriot_font", "./assets/fonts/charriot.ttf", 42);
	assetManager->AddFont("pico12_font", "./assets/fonts/pico8.ttf", 12);
	assetManager->AddFont("pico14_font", "./assets/fonts/pico8.ttf", 14);

	Logger::Err("Assets Loaded");

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
			tile.Group("tile");
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize),
				y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0f);

			tile.AddComponent<SpriteComponent>("tilemap_jungle", tileSize, tileSize, 0, false, srcRectX, srcRectY);
		}
	}
	jungleMap.close();
	mapWidth = mapNumColumns * tileSize * tileScale;
	mapHeight = mapNumRows * tileSize * tileScale;


	Entity chopper = registry->CreateEntity();
	chopper.AddComponent<TransformComponent>(glm::vec2(500.0f, 500.0f), glm::vec2(4.0f, 4.0f), 0.0f);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0f, 0.0f));
	chopper.AddComponent<SpriteComponent>("chopper_img", 32, 32, 3);
	chopper.AddComponent<AnimationComponent>(2, 8, true);
	chopper.AddComponent<KeyboardControlComponent>(glm::vec2(0, -2000), glm::vec2(2000, 0), glm::vec2(0, 2000), glm::vec2(-2000, 0));
	chopper.AddComponent<CameraFollowComponent>();
	chopper.AddComponent<HealthComponent>(100);
	chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(3000.0, 3000.0), 0, 10000, 25, true);
	chopper.AddComponent<BoxColliderComponent>(32 * 4, 32 * 4);
	chopper.Tag("player");

	Entity radar = registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(1500.0f, 50.0f), glm::vec2(1.0f, 1.0f), 0.0f);
	radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0f, 0.0f));
	radar.AddComponent<SpriteComponent>("radar_img", 64, 64, 99, true);
	radar.AddComponent<AnimationComponent>(8, 4, true);

	Entity tank = registry->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(11.0f, 11.0f), glm::vec2(5.0f, 5.0f), 33.0f);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(0.0f, 0.0f));
	tank.AddComponent<SpriteComponent>("tank_panther_right", 32, 32, 1);
	tank.AddComponent<BoxColliderComponent>(32 * tank.GetComponent<TransformComponent>().scale.x,
									 		32 * tank.GetComponent<TransformComponent>().scale.y);
	tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(2000.0, 2000.0), 2000, 10000, 10);
	tank.AddComponent<HealthComponent>(100);
	tank.Group("enemy");


	Entity antitank = registry->CreateEntity();
	antitank.Group("enemy");
	antitank.AddComponent<TransformComponent>(glm::vec2(1400.0f, 1000.0f), glm::vec2(5.0f, 5.0f), 21.0f);
	antitank.AddComponent<RigidBodyComponent>(glm::vec2(0.0f, 0.0f));
	antitank.AddComponent<SpriteComponent>("tank_panther_left", 32, 32, 1);
	// bounding box not considering scale of object, need to think about it
	antitank.AddComponent<BoxColliderComponent>(32 * antitank.GetComponent<TransformComponent>().scale.x,
											 32 * antitank.GetComponent<TransformComponent>().scale.y);
	antitank.AddComponent<ProjectileEmitterComponent>(glm::vec2(-1800.0, -1800.0), 3000, 10000, 30);
	antitank.AddComponent<HealthComponent>(200);

	Entity textTest = registry->CreateEntity();
	SDL_Color colorTest = {55, 222, 111};
	textTest.AddComponent<TextComponent>(glm::vec2(windowWidth / 3, 111), "THIS IS MIGHT BEE SOME USEFULL TEXT, But It's Just A Placeholder [-_- ] ", "charriot_font", colorTest);

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
		if (isDebug)
		{
			ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
			ImGuiIO& inputOutput = ImGui::GetIO();

			int32_t mouseX, mouseY;
			const int32_t buttons = SDL_GetMouseState(&mouseX, &mouseY);

			inputOutput.MousePos = ImVec2(mouseX, mouseY);
			inputOutput.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
			inputOutput.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
		}

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
				if(sdlEvent.key.keysym.sym == SDLK_c)
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
	registry->GetSystem<ProjectileEmitterSystem>().SubscribeToEvents(eventBus);

	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CollisionSystem>().Update(eventBus);
	registry->GetSystem<CameraMovementSystem>().Update(camera);
	registry->GetSystem<ProjectileEmitterSystem>().Update(registry);
	registry->GetSystem<ProjectileLifeCycleSystem>().Update();

	// At the End of Frame add/remove entities in que to proceed
	registry->Update();


}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 42, 42, 42, 255);
	SDL_RenderClear(renderer);

	registry->GetSystem<RenderSystem>().Update(renderer, assetManager, camera);
	if(isDebug)
	{
		registry->GetSystem<CollisionSystem>().RenderBoxCollision(renderer, camera);
		registry->GetSystem<RenderGuiSystem>().Update(registry, camera);
	}
	registry->GetSystem<RenderHealthBarSystem>().Update(renderer, assetManager, camera);
	registry->GetSystem<RenderTextSystem>().Update(renderer , assetManager, camera);

	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	Logger::Err("It's Dead |-_-\"|");
}