#include <stdio.h>
#include <stdbool.h>
#include "KeyboardController.h"
#include "ResourceController.h"

void HandleKeyboardEvents(SdlParameters* sdlParameters, GameResources* gameResources)
{
	if (sdlParameters->event.type == SDL_KEYDOWN)	// If a key is pressed down (Oneshot). Need to add debounce logic.
	{
		if (DEBUG) printf("[DEBUG INFO] SDL_KEYDOWN event occurred. KEY: %s\n", SDL_GetKeyName(sdlParameters->event.key.keysym.sym));
		switch (sdlParameters->event.key.keysym.sym)
		{
		case SDLK_ESCAPE:	// Press 'ESCAPE' key to exit game (Need to improve this by adding a button to exit the game)
			sdlParameters->mainLoopRunning = false;
			return;
		case SDLK_r:		// Press 'R' to reset game state (For Testing)
			ResetGameResourceParameters(RESET_BACKGROUND | RESET_FLOOR, gameResources, sdlParameters);
			break;
		case SDLK_SPACE:
			if (gameResources->gameState == GAME_RUNNING)
			{
				gameResources->bird->destRect.y -= gameResources->bird->gravity * 12;
				gameResources->bird->gravity = 0;
			}
			break;
		default:
			if(DEBUG) printf("[DEBUG INFO] No logic provided to handle when key %s is pressed!\n", SDL_GetKeyName(sdlParameters->event.key.keysym.sym));
			break;
		}
	}
	else if (sdlParameters->event.type == SDL_KEYUP)	// If a key is released (Oneshot)
	{
		if (DEBUG) printf("[DEBUG INFO] SDL_KEYUP Event occurred.\n");
	}
	return;
}
