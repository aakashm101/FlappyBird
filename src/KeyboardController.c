#include <stdio.h>
#include <stdbool.h>
#include "KeyboardController.h"

void handleKeyboardEvents(SdlParameters* sdlParameters)
{
	if (sdlParameters->event.type == SDL_KEYDOWN)	// If a key is pressed down (Oneshot)
	{
		if (DEBUG) printf("[DEBUG INFO] SDL_KEYDOWN Event occurred. KEY: %s\n", SDL_GetKeyName(sdlParameters->event.key.keysym.sym));
		switch (sdlParameters->event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			sdlParameters->mainLoopRunning = false;
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
