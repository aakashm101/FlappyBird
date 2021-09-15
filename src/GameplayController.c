#include "GameplayController.h"
#include "SDLController.h"
#include "ResourceController.h"
#include "global.h"

void HandleGameplay(SdlParameters* sdlParameters, GameResources* gameResources)
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
		// Fetch the values once instead of accessing it multiple times
		int left = gameResources->pillarPairsLeftEndIndex;
		int right = gameResources->pillarPairsRightEndIndex;
		int rightEndTopPillarXpos = gameResources->pillarPairs[right].topPillar->destRect.x;
		int rightEndTopPillarWidth = gameResources->pillarPairs[right].topPillar->destRect.w;
		int rightEndBottomPillarXpos = gameResources->pillarPairs[right].bottomPillar->destRect.x;
		int rightEndBottomPillarWidth = gameResources->pillarPairs[right].bottomPillar->destRect.w;

		(gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x) += gameResources->pillarPairs[pillarPairIndex].topPillar->xTranslation;
		(gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x) += gameResources->pillarPairs[pillarPairIndex].bottomPillar->xTranslation;

		// Avoiding the bird from going out of the window or below the floor
		if (gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x + gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.w < 0)
		{
			gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x = rightEndTopPillarXpos + rightEndTopPillarWidth + gameResources->distanceBetweenPillars;
		}
		if (gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x + gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.w < 0)
		{
			gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x = rightEndBottomPillarXpos + rightEndBottomPillarWidth + gameResources->distanceBetweenPillars;
		}
		gameResources->pillarPairsLeftEndIndex = (left + 1) % gameResources->pillarPairCount;
		gameResources->pillarPairsRightEndIndex = (right + 1) % gameResources->pillarPairCount;

		CopySpriteToRenderer(gameResources->pillarPairs[pillarPairIndex].topPillar, sdlParameters, gameResources);
		CopySpriteToRenderer(gameResources->pillarPairs[pillarPairIndex].bottomPillar, sdlParameters, gameResources);
	}

	// Copy the bird sprite to the renderer
	if (gameResources->bird->destRect.y + gameResources->bird->destRect.h + gameResources->floorSpriteArray[0].destRect.h < sdlParameters->dm.h)
	{
		gameResources->bird->yTranslation = gameResources->bird->gravity;
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

		// Limit gravity and lift values
		if (gameResources->bird->gravity < gameResources->bird->maxGravity)
		{
			(gameResources->bird->gravity)++;
		}
		else
		{
			gameResources->bird->gravity = gameResources->bird->maxGravity;
		}
		if (gameResources->bird->lift < gameResources->bird->maxLift)
		{
			(gameResources->bird->lift)++;
		}
		else
		{
			gameResources->bird->lift = gameResources->bird->maxLift;
		}
	}
	CopySpriteToRenderer(gameResources->bird, sdlParameters, gameResources);

	// Display the floor and do parallax effect on the floor
	for (int floorSpriteIndex = 0; floorSpriteIndex < gameResources->floorSpriteCount; floorSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->floorSpriteArray[floorSpriteIndex]), sdlParameters, gameResources);
	}
	ParallaxEffect(gameResources->floorSpriteArray, gameResources->floorSpriteCount, &(gameResources->floorLeftEndIndex), &(gameResources->floorRightEndIndex));

	// For each pillar pair, check for collisions
	for (int pillarPairIndex = 0; pillarPairIndex < gameResources->pillarPairCount; pillarPairIndex++)
	{
		CheckSpriteCollision(gameResources->pillarPairs[pillarPairIndex].topPillar, gameResources->bird);
		CheckSpriteCollision(gameResources->pillarPairs[pillarPairIndex].bottomPillar, gameResources->bird);
	}

	return;
}

// Used to detect whether two sprites are colliding
static bool CheckSpriteCollision(Sprite* sprite1, Sprite* sprite2)
{
	// Check whether top left corner of B is within area of A
	if (sprite2->destRect.x >= sprite1->destRect.x && sprite2->destRect.x <= sprite1->destRect.x + sprite1->destRect.w &&
		sprite2->destRect.y >= sprite1->destRect.y && sprite2->destRect.y <= sprite1->destRect.y + sprite1->destRect.h)
	{
		if (DEBUG) printf("[DEBUG INFO] Top left corner of %s colliding with %s\n", sprite2->name, sprite1->name);
		return true;
	}

	// Check whether top right corner of B is within area of A
	else if (sprite2->destRect.x + sprite2->destRect.w >= sprite1->destRect.x && sprite2->destRect.x <= sprite1->destRect.x + sprite1->destRect.w &&
		sprite2->destRect.y >= sprite1->destRect.y && sprite2->destRect.y <= sprite1->destRect.y + sprite1->destRect.h)
	{
		if (DEBUG) printf("[DEBUG INFO] Top right corner of %s colliding with %s\n", sprite2->name, sprite1->name);
		return true;
	}
	// Check whether bottom left corner of B is within area of A
	else if (sprite2->destRect.x >= sprite1->destRect.x && sprite2->destRect.x <= sprite1->destRect.x + sprite1->destRect.w &&
		sprite2->destRect.y + sprite2->destRect.h >= sprite1->destRect.y && sprite2->destRect.y + sprite2->destRect.h <= sprite1->destRect.y + sprite1->destRect.h)
	{
		if (DEBUG) printf("[DEBUG INFO] Bottom left corner of %s colliding with %s\n", sprite2->name, sprite1->name);
		return true;
	}
	// Check whether bottom right corner of B is within area of A
	else if (sprite2->destRect.x + sprite2->destRect.w >= sprite1->destRect.x && sprite2->destRect.x + sprite2->destRect.w <= sprite1->destRect.x + sprite1->destRect.w &&
		sprite2->destRect.y + sprite2->destRect.h >= sprite1->destRect.y && sprite2->destRect.y + sprite2->destRect.h <= sprite1->destRect.y + sprite1->destRect.h)
	{
		if (DEBUG) printf("[DEBUG INFO] Bottom right corner of %s colliding with %s\n", sprite2->name, sprite1->name);
		return true;
	}

	return false;
}
