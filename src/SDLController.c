#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "SDLController.h"

int SDL_Start(SdlParameters* sdlParameters)
{
	const char* WINDOW_TITLE = "Flappy Bird Clone - by Aakash.M";
	const char* FONT_FILE_PATH = "res/OpenSans-Regular.ttf";

	sdlParameters->FPS = 60;
	sdlParameters->mainLoopRunning = true;
	sdlParameters->fontPath = FONT_FILE_PATH;
	sdlParameters->window = NULL;
	sdlParameters->renderer = NULL;

	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("[ERROR] SDL_Init() failed!\n");
		return -1;
	}
	if(DEBUG) printf("[DEBUG INFO] SDL_Init() successful.\n");

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		printf("[ERROR] IMG_Init() failed!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] IMG_Init() successful.\n");

	if (TTF_Init() != 0)
	{
		printf("[ERROR] TTF_Init() failed!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] TTF_Init() successful.\n");

	if (SDL_GetCurrentDisplayMode(0, &(sdlParameters->dm)) != 0)
	{
		printf("[ERROR] Failed to fetch display mode!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] SDL_GetCurrentDisplayMode() successful.\n");

	sdlParameters->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sdlParameters->dm.w, sdlParameters->dm.h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
	if (!sdlParameters->window)
	{
		printf("[ERROR] SDL_CreateWindow() failed!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] SDL_CreateWindow() successful.\n");

	sdlParameters->renderer = SDL_CreateRenderer(sdlParameters->window, -1, SDL_RENDERER_ACCELERATED);
	if (!sdlParameters->renderer)
	{
		printf("[ERROR] SDL_CreateRenderer() failed!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] SDL_CreateRenderer() successful.\n");

	return 0;
}

void SDL_End(SdlParameters* sdlParameters)
{
	if (sdlParameters->renderer)
	{
		SDL_DestroyRenderer(sdlParameters->renderer);
		sdlParameters->renderer = NULL;
		if (DEBUG) printf("[DEBUG INFO] SDL_DestroyRenderer() successful.\n");
	}
	
	if (sdlParameters->window)
	{
		SDL_DestroyWindow(sdlParameters->window);
		sdlParameters->window = NULL;
		if (DEBUG) printf("[DEBUG INFO] SDL_DestroyWindow() successful.\n");
	}

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return;
}

void CopySpriteToRenderer(Sprite* sprite, SdlParameters* const sdlParameters, const GameResources* const gameResources)
{
	SDL_RenderCopyEx(
		sdlParameters->renderer,
		gameResources->tileMap,
		&(sprite->srcRect),
		&(sprite->destRect),
		sprite->angle,
		NULL,
		SDL_FLIP_NONE);
	return;
}