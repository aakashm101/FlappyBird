#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define SDL_MAIN_DEFINED
#include <SDL2/SDL.h>
#include "global.h"
#include "SDLController.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include "ResourceController.h"
#include "MainMenuController.h"
#include "GameplayController.h"
#include "LeaderboardController.h"

static SdlParameters sdlParameters;
static GameResources gameResources;

int DEBUG = 0;

int main()
{
	if (SDL_Start(&sdlParameters) != 0)
	{
		printf("[ERROR] Failed to start SDL!\n");
		SDL_End(&sdlParameters);
		printf("Press any key to exit...\n");
		getchar();
		return -1;
	}
	
	srand(time(NULL));	// Use time as seed
	if (LoadGameResources(&gameResources, &sdlParameters) != 0)
	{
		SDL_End(&sdlParameters);
		printf("[ERROR] Error loading game resources!\n");
		printf("Press any key to exit...\n");
		getchar();
		return -1;
	}

	while (sdlParameters.mainLoopRunning)
	{
		Uint32 frameStart, frameDelay;
		const Uint32 FRAME_TIME = 1000 / sdlParameters.FPS;	// FPS value is set when SDL_Start is called (Default: 30 FPS)

		frameStart = SDL_GetTicks();
		if (SDL_PollEvent(&(sdlParameters.event)))
		{
			if (sdlParameters.event.type == SDL_QUIT)
			{
				sdlParameters.mainLoopRunning = false;
				break;
			}
			else if (sdlParameters.event.type == SDL_KEYDOWN || sdlParameters.event.type == SDL_KEYUP)
			{
				HandleKeyboardEvents(&sdlParameters, &gameResources);
			}
			else if (sdlParameters.event.type == SDL_MOUSEBUTTONDOWN)
			{
				HandleMouseEvents(&sdlParameters, &gameResources);
			}
		}

		SDL_RenderClear(sdlParameters.renderer);
		switch (gameResources.gameState)
		{
		case GAME_MAIN_MENU:
			ShowMainMenu(&gameResources, &sdlParameters);
			break;
		case GAME_LEADERBOARD:
			HandleLeaderboard(&sdlParameters, &gameResources);
			break;
		case GAME_RUNNING:
			HandleGameplay(&sdlParameters, &gameResources);
			break;
		case GAME_OVER:
			HandleGameOver(&sdlParameters, &gameResources);
			break;
		default:
			break;
		}
		SDL_RenderPresent(sdlParameters.renderer);

		frameDelay = SDL_GetTicks() - frameStart;
		if (frameDelay < FRAME_TIME)
		{
			SDL_Delay(FRAME_TIME - frameDelay);
		}
	}

	SDL_End(&sdlParameters);
	UnloadGameResources(&gameResources);
	if (DEBUG) printf("[DEBUG INFO] Exiting program.\n");
	return 0;
}