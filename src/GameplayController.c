#include "GameplayController.h"
#include "ResourceController.h"
#include "global.h"

void handleGameplay(SdlParameters* sdlParameters, GameResources* gameResources)
{
	// Display the backgrounds and do parallax effect on the background
	for (int backgroundSpriteIndex = 0; backgroundSpriteIndex < gameResources->backgroundSpriteCount; backgroundSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->backgroundSpriteArray[backgroundSpriteIndex]), sdlParameters, gameResources);
	}
	ParallaxEffect(gameResources->backgroundSpriteArray, gameResources->backgroundSpriteCount, &(gameResources->backgroundLeftEndIndex), &(gameResources->backgroundRightEndIndex));
	
	// Rendering multiple pillars
	for (int pillarPairIndex = 0; pillarPairIndex < gameResources->pillarPairCount; pillarPairIndex++)
	{
		(gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x)--;
		(gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x)--;

		if (gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x + gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.w < 0)
		{
			gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x = gameResources->pillarPairs[gameResources->pillarPairsRightEndIndex].topPillar->destRect.x + gameResources->pillarPairs[gameResources->pillarPairsRightEndIndex].topPillar->destRect.w + gameResources->distanceBetweenPillars;
			gameResources->pillarPairsLeftEndIndex = (gameResources->pillarPairsLeftEndIndex + 1) % gameResources->pillarPairCount;
		}
		if (gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x + gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.w < 0)
		{
			gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x = gameResources->pillarPairs[gameResources->pillarPairsRightEndIndex].bottomPillar->destRect.x + gameResources->pillarPairs[gameResources->pillarPairsRightEndIndex].bottomPillar->destRect.w + gameResources->distanceBetweenPillars;
			gameResources->pillarPairsRightEndIndex = (gameResources->pillarPairsRightEndIndex + 1) % gameResources->pillarPairCount;
		}

		CopySpriteToRenderer(gameResources->pillarPairs[pillarPairIndex].topPillar, sdlParameters, gameResources);
		CopySpriteToRenderer(gameResources->pillarPairs[pillarPairIndex].bottomPillar, sdlParameters, gameResources);
	}

	// Display the floor and do parallax effect on the floor
	for (int floorSpriteIndex = 0; floorSpriteIndex < gameResources->floorSpriteCount; floorSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->floorSpriteArray[floorSpriteIndex]), sdlParameters, gameResources);
	}
	ParallaxEffect(gameResources->floorSpriteArray, gameResources->floorSpriteCount, &(gameResources->floorLeftEndIndex), &(gameResources->floorRightEndIndex));

	return;
}
