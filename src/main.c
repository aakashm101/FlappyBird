#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define SDL_MAIN_DEFINED
#include <SDL2/SDL.h>
#include "global.h"
#include "SDLController.h"
#include "KeyboardController.h"
#include "ResourceController.h"
#include "MainMenuController.h"

static SdlParameters sdlParameters;
static GameResources gameResources;

int DEBUG = 1;

int main(void)
{
	if (SDL_Start(&sdlParameters) != 0)
	{
		printf("[ERROR] Failed to start SDL!\n");
		SDL_End(&sdlParameters);
	}
	
	LoadGameResources(&gameResources, &sdlParameters);
	gameResources.gameState = GAME_MAIN_MENU;	// Open the main menu on start
	
	SDL_SetRenderDrawColor(sdlParameters.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
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
				handleKeyboardEvents(&sdlParameters);
			}
		}

		SDL_RenderClear(sdlParameters.renderer);
		switch (gameResources.gameState)
		{
		case GAME_MAIN_MENU:
			showMainMenu(&gameResources, &sdlParameters);
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
	return 0;
}