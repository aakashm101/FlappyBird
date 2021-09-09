#include "GameplayController.h"
#include "SDLController.h"
#include "ResourceController.h"
#include "global.h"

void handleGameplay(SdlParameters* sdlParameters, GameResources* gameResources)
{
	// Fetch the value once instead of accessing it multiple times
	int distanceBetweenPillars = gameResources->distanceBetweenPillars;
	int pillarPairCount = gameResources->pillarPairCount;

	// Display the backgrounds and do parallax effect on the background
	for (int backgroundSpriteIndex = 0; backgroundSpriteIndex < gameResources->backgroundSpriteCount; backgroundSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->backgroundSpriteArray[backgroundSpriteIndex]), sdlParameters, gameResources);
	}
	ParallaxEffect(gameResources->backgroundSpriteArray, gameResources->backgroundSpriteCount, &(gameResources->backgroundLeftEndIndex), &(gameResources->backgroundRightEndIndex));
	
	// Rendering multiple pillars
	for (int pillarPairIndex = 0; pillarPairIndex < gameResources->pillarPairCount; pillarPairIndex++)
	{
		// Fetch the values once instead of accessing it multiple times
		int left = gameResources->pillarPairsLeftEndIndex;
		int right = gameResources->pillarPairsRightEndIndex;
		int rightEndTopPillarXpos = gameResources->pillarPairs[right].topPillar->destRect.x;
		int rightEndTopPillarWidth = gameResources->pillarPairs[right].topPillar->destRect.w;
		int rightEndBottomPillarXpos = gameResources->pillarPairs[right].bottomPillar->destRect.x;
		int rightEndBottomPillarWidth = gameResources->pillarPairs[right].bottomPillar->destRect.w;

		(gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x) += gameResources->pillarPairs[pillarPairIndex].topPillar->xTranslation;
		(gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x) += gameResources->pillarPairs[pillarPairIndex].bottomPillar->xTranslation;

		if (gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x + gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.w < 0)
		{
			gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x = rightEndTopPillarXpos + rightEndTopPillarWidth + distanceBetweenPillars;
		}
		if (gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x + gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.w < 0)
		{
			gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x = rightEndBottomPillarXpos + rightEndBottomPillarWidth + distanceBetweenPillars;
		}
		gameResources->pillarPairsLeftEndIndex = (left + 1) % pillarPairCount;
		gameResources->pillarPairsRightEndIndex = (right + 1) % pillarPairCount;

		CopySpriteToRenderer(gameResources->pillarPairs[pillarPairIndex].topPillar, sdlParameters, gameResources);
		CopySpriteToRenderer(gameResources->pillarPairs[pillarPairIndex].bottomPillar, sdlParameters, gameResources);
	}

	// Copy the bird sprite to the renderer
	if (gameResources->bird->destRect.y + gameResources->bird->destRect.h + gameResources->floorSpriteArray[0].destRect.h < sdlParameters->dm.h)
	{
		gameResources->bird->yTranslation += gameResources->bird->gravity;
		gameResources->bird->yTranslation -= gameResources->bird->lift;
		
		gameResources->bird->destRect.y += gameResources->bird->yTranslation;
		if (gameResources->bird->destRect.y + gameResources->bird->destRect.h + gameResources->floorSpriteArray[0].destRect.h > sdlParameters->dm.h)
		{
			gameResources->bird->destRect.y = sdlParameters->dm.h - (gameResources->bird->destRect.h + gameResources->floorSpriteArray[0].destRect.h);
		}
		else if (gameResources->bird->destRect.y < 0)
		{
			gameResources->bird->destRect.y = 0;
		}

		if (gameResources->bird->gravity < gameResources->bird->maxGravity)
		{
			(gameResources->bird->gravity)++;
		}
		if (gameResources->bird->lift < gameResources->bird->maxLift)
		{
			(gameResources->bird->lift)++;
		}
	}

	CopySpriteToRenderer(gameResources->bird, sdlParameters, gameResources);

	// Display the floor and do parallax effect on the floor
	for (int floorSpriteIndex = 0; floorSpriteIndex < gameResources->floorSpriteCount; floorSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->floorSpriteArray[floorSpriteIndex]), sdlParameters, gameResources);
	}
	ParallaxEffect(gameResources->floorSpriteArray, gameResources->floorSpriteCount, &(gameResources->floorLeftEndIndex), &(gameResources->floorRightEndIndex));

	return;
}
