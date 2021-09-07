#include "GameplayController.h"

void handleGameplay(SdlParameters* sdlParameters, GameResources* gameResources)
{
	for (int backgroundSpriteIndex = 0; backgroundSpriteIndex < gameResources->backgroundSpriteCount; backgroundSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->backgroundSpriteArray[backgroundSpriteIndex]), sdlParameters, gameResources);
	}
	ParallaxEffect(gameResources->backgroundSpriteArray, gameResources->backgroundSpriteCount, gameResources);

	SDL_RenderCopy(
		sdlParameters->renderer,
		gameResources->comingSoonText->textTexture,
		NULL,
		&(gameResources->comingSoonText->destRect));
	return;
}
