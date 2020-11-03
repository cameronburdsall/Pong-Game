#pragma once
#include "SDL/SDL.h"


struct myVector
{
	//struct for storing coordinates
	float x;
	float y;
};


class myGame
{
public:
	myGame();
	//Initializes Game, returns true if successful
	bool Initialize();
	//Runs Game Loop
	void RunLoop();
	//Stops and deconstructs Game
	void Shutdown();
private:
	//Game Loop Functions
	void ProcessInputs();
	void UpdateGame();
	void GenerateOutput();
	//Member Variables
	//SDL Window
	SDL_Window* GameWindow;
	//Bool variable to keep track of game status
	bool IsRunning;
	//Renderer Object
	SDL_Renderer* GameRenderer;
	//Vectors for ball and paddle Position
	myVector PaddlePosition;
	myVector BallPosition;
	myVector BallVelocity;
	//Interger tracks paddle direction
	int PaddleDirection;
	//SDL Tick counter
	Uint32 TickCount;
};