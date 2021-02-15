#include <iostream>
#include "Constants.h"
#include "Game.h"




int main(int argc, char *args[])
{
	sol::state lua;
	Game *game = new Game();
	game->Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	while(game->IsRunning())
	{
		game->ProcessInput();
		game->Update();
		game->Render();
	}
	game->Destroy();
	delete game;
	return(0);
}