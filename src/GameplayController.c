#include "GameplayController.h"
#include "SDLController.h"

void handleGameplay(SdlParameters* sdlParameters, GameResources* gameResources)
{
	// Display the backgrounds and do parallax effect on the background
	for (int backgroundSpriteIndex = 0; backgroundSpriteIndex < gameResources->backgroundSpriteCount; backgroundSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->backgroundSpriteArray[backgroundSpriteIndex]), sdlParameters, gameResources);
	}
	ParallaxEffect(gameResources->backgroundSpriteArray, gameResources->backgroundSpriteCount, &(gameResources->backgroundLeftEndIndex), &(gameResources->backgroundRightEndIndex));

	// Display the floor and do parallax effect on the floor
	for (int floorSpriteIndex = 0; floorSpriteIndex < gameResources->floorSpriteCount; floorSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->floorSpriteArray[floorSpriteIndex]), sdlParameters, gameResources);
	}
	ParallaxEffect(gameResources->floorSpriteArray, gameResources->floorSpriteCount, &(gameResources->floorLeftEndIndex), &(gameResources->floorRightEndIndex));

	// Testing: Two pillars on the screen
	CopySpriteToRenderer(gameResources->topPillar, sdlParameters, gameResources);
	CopySpriteToRenderer(gameResources->bottomPillar, sdlParameters, gameResources);
	
	SDL_RenderCopy(sdlParameters->renderer, gameResources->comingSoonText->textTexture, NULL, &(gameResources->comingSoonText->destRect));
	return;
}
