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


	// Testing: Moving two pillars on the screen
	if (gameResources->topPillar->destRect.x > 0)
	{
		(gameResources->topPillar->destRect.x) -= 1;
	}
	else
	{
		gameResources->topPillar->destRect.x = sdlParameters->dm.w;
	}
	
	if (gameResources->bottomPillar->destRect.x > 0)
	{
		(gameResources->bottomPillar->destRect.x) -= 1;
	}
	else
	{
		gameResources->bottomPillar->destRect.x = sdlParameters->dm.w;
	}
	CopySpriteToRenderer(gameResources->topPillar, sdlParameters, gameResources);
	CopySpriteToRenderer(gameResources->bottomPillar, sdlParameters, gameResources);

	// Display the floor and do parallax effect on the floor
	for (int floorSpriteIndex = 0; floorSpriteIndex < gameResources->floorSpriteCount; floorSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->floorSpriteArray[floorSpriteIndex]), sdlParameters, gameResources);
	}
	ParallaxEffect(gameResources->floorSpriteArray, gameResources->floorSpriteCount, &(gameResources->floorLeftEndIndex), &(gameResources->floorRightEndIndex));

	return;
}
