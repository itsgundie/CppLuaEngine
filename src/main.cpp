#include <iostream>
#include "Constants.h"
#include "Game.h"




int main(int argc, char *argv[])
{
	Game *game = new Game();
	game->Init();
	game->Run();
	game->Destroy();
	delete game;
	return(0);
}