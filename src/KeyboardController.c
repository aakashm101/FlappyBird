#include <stdio.h>
#include <stdbool.h>
#include "KeyboardController.h"
#include "ResourceController.h"

void handleKeyboardEvents(SdlParameters* sdlParameters, GameResources* gameResources)
{
	if (sdlParameters->event.type == SDL_KEYDOWN)	// If a key is pressed down (Oneshot). Need to add debounce logic.
	{
		if (DEBUG) printf("[DEBUG INFO] SDL_KEYDOWN event occurred. KEY: %s\n", SDL_GetKeyName(sdlParameters->event.key.keysym.sym));
		switch (sdlParameters->event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			sdlParameters->mainLoopRunning = false;
			return;
		case SDLK_SPACE:
			if (gameResources->gameState == GAME_RUNNING)
			{
				gameResources->bird->destRect.y -= gameResources->bird->gravity * 12;
				gameResources->bird->gravity = 0;
			}
			break;
		default:
			printf("[ERROR] No logic provided to handle when key %s is pressed!\n", SDL_GetKeyName(sdlParameters->event.key.keysym.sym));
			break;
		}
	}
	else if (sdlParameters->event.type == SDL_KEYUP)	// If a key is released (Oneshot)
	{
		if (DEBUG) printf("[DEBUG INFO] SDL_KEYUP Event occurred.\n");
	}
	return;
}
