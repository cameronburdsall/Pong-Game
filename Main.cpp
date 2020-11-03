#include "myGame.h"

int main(int argc, char** argv)
{
	myGame game;
	//Game game;
	bool success = game.Initialize();
	if (success)
	{
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}
