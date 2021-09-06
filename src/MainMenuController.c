#include "MainMenuController.h"

void showMainMenu(GameResources* gameResources, SdlParameters* sdlParameters)
{
	for (int backgroundSpriteIndex = 0; backgroundSpriteIndex < gameResources->backgroundSpriteCount; backgroundSpriteIndex++)
	{
		SDL_RenderCopyEx(
			sdlParameters->renderer,
			gameResources->tileMap,
			&(gameResources->backgroundSpriteArray[backgroundSpriteIndex].srcRect),
			&(gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect),
			gameResources->backgroundSpriteArray[backgroundSpriteIndex].angle,
			NULL,
			SDL_FLIP_NONE);
	}

	SDL_RenderCopyEx(
		sdlParameters->renderer, 
		gameResources->tileMap, 
		&(gameResources->flappyBirdLogo->srcRect), 
		&(gameResources->flappyBirdLogo->destRect),
		gameResources->flappyBirdLogo->angle, 
		NULL, 
		SDL_FLIP_NONE);
}