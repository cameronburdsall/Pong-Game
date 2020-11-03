#include "myGame.h"

/*
	Window Macros
					*/
#define WINDOW_NAME "Simple Pong Game"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define WINDOW_X 100
#define WINDOW_Y 100
/*
	Renderer Macros
					*/
//Background Colors
#define BACKGROUND_R 0
#define BACKGROUND_G 200
#define BACKGROUND_B 30
#define BACKGROUND_A 255
//Rectangle Colors
#define RECT_R 0
#define RECT_G 0
#define RECT_B 0
#define RECT_A 255
//Rectangle Size Values
#define THICKNESS 15
#define PADDLE_HEIGHT 100.0f



myGame::myGame():
	GameWindow(nullptr),
	GameRenderer(nullptr),
	TickCount(0),
	IsRunning (true),
	PaddleDirection(0)
{
}

bool myGame::Initialize()
{
	int SDLInitTest = SDL_Init(SDL_INIT_VIDEO);
	if (SDLInitTest != 0)
		//If Initialization Fails, report error and return failure
	{
		SDL_Log("Initialization Failed: %s", SDL_GetError());
		return false;
	}
	GameWindow = SDL_CreateWindow(
		WINDOW_NAME, //Name
		WINDOW_X,			//X coordinate
		WINDOW_Y,			//Y coordinate
		WINDOW_WIDTH,		//Window Width
		WINDOW_HEIGHT,		//Window Height
		0					//Flags
	);
	if (!GameWindow)
		//Check if Window was properly created
	{
		SDL_Log("Window Creation Failed: %s", SDL_GetError());
		return false;
	}
	GameRenderer = SDL_CreateRenderer(
		GameWindow,		//Target Window
		-1,				//Renderer Driver index
		//Renderer Flags
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!GameRenderer)
	{
		SDL_Log("Game Renderer Failure: %s", SDL_GetError());
		return false;
	}

	PaddlePosition.x = 10.0f;
	PaddlePosition.y = 768.0f / 2.0f;
	BallPosition.x = 1024.0f / 2.0f;
	BallPosition.y = 768.0f / 2.0f;
	BallVelocity.x = -200.0f;
	BallVelocity.y = 235.0f;

	return true;
}

void myGame::Shutdown()
{
	SDL_DestroyWindow(GameWindow);
	SDL_DestroyRenderer(GameRenderer);
	SDL_Quit();
}

void myGame::RunLoop()
{
	while (IsRunning == true)
	{
		ProcessInputs();
		UpdateGame();
		GenerateOutput();
	}
}

void myGame::ProcessInputs()
{
	//Creates event variable, accesses events queue
	SDL_Event events;
	while (SDL_PollEvent(&events))
		//while there are still events in the events queue
	{
		switch (events.type)
			//switch cases are different events we need to deal with
		{
			case SDL_QUIT:
				IsRunning = false;
				break;
		}
	}
	const Uint8* KeyboardState = SDL_GetKeyboardState(NULL);

	//if statements process key presses

	//Close game upon pressing escape
	if (KeyboardState[SDL_SCANCODE_ESCAPE]) IsRunning = false;

	//Change Direction of paddle
	PaddleDirection = 0;
	if (KeyboardState[SDL_SCANCODE_W]) PaddleDirection -= 1;
	if (KeyboardState[SDL_SCANCODE_S]) PaddleDirection += 1;


}

void myGame::GenerateOutput()
{
	//Set background
	SDL_SetRenderDrawColor(
		GameRenderer,
		BACKGROUND_R,
		BACKGROUND_G,
		BACKGROUND_B,
		BACKGROUND_A
	);
	SDL_RenderClear(GameRenderer);
	//Set rectangle color
	SDL_SetRenderDrawColor(
		GameRenderer,
		RECT_R,
		RECT_G,
		RECT_B,
		RECT_A
	);
	//declare top and bottom walls
	SDL_Rect wall{
		0,
		0,
		WINDOW_WIDTH,
		THICKNESS
	};
	SDL_Rect wall2{
		0,
		WINDOW_HEIGHT - THICKNESS,
		WINDOW_WIDTH,
		THICKNESS
	};
	SDL_RenderFillRect(GameRenderer, &wall);
	SDL_RenderFillRect(GameRenderer, &wall2);

	SDL_Rect ball{
		static_cast<int>(BallPosition.x - THICKNESS/2),
		static_cast<int>(BallPosition.y - THICKNESS/2),
		THICKNESS,
		THICKNESS
	};
	SDL_Rect paddle{
		static_cast<int>(PaddlePosition.x - THICKNESS / 2),
		static_cast<int>(PaddlePosition.y - PADDLE_HEIGHT / 2),
		THICKNESS,
		static_cast<int>(PADDLE_HEIGHT)
	};
	SDL_RenderFillRect(GameRenderer, &ball);
	SDL_RenderFillRect(GameRenderer, &paddle);
	//Draw game scene
	SDL_RenderPresent(GameRenderer);
}

void myGame::UpdateGame()
{
	//waits for 16ms to pass
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), TickCount + 16));

	float DeltaTime = (SDL_GetTicks() - TickCount) / 1000.0f;

	//caps DeltaTime at 0.05f
	if (DeltaTime > 0.05f) DeltaTime = 0.05f;


	TickCount = SDL_GetTicks();

	//Change Paddle Position
	if (PaddleDirection != 0)
	{
		PaddlePosition.y += PaddleDirection * 300.0f * DeltaTime;
		if (PaddlePosition.y < (PADDLE_HEIGHT / 2.0f + THICKNESS))
			PaddlePosition.y = PADDLE_HEIGHT / 2.0f + THICKNESS;
		else if (PaddlePosition.y > (WINDOW_HEIGHT - PADDLE_HEIGHT / 2.0f - THICKNESS))
			PaddlePosition.y = WINDOW_HEIGHT - PADDLE_HEIGHT / 2.0f - THICKNESS;

	}
	//Change Ball Position
	BallPosition.x = BallVelocity.x * DeltaTime;
	BallPosition.y = BallVelocity.y * DeltaTime;
	//Top Wall Collision
	if (BallPosition.y <= THICKNESS && BallVelocity.y < 0.0f)
		BallVelocity.y *= -1;
	else if (BallPosition.y >= (WINDOW_HEIGHT - THICKNESS) && BallVelocity.y > 0.0f)
		BallVelocity.y *= -1;
}