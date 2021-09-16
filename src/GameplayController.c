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
		int left = gameResources->pillarPairsLeftEndIndex;
		int right = gameResources->pillarPairsRightEndIndex;
		int rightEndTopPillarXpos = gameResources->pillarPairs[right].topPillar->destRect.x;
		int rightEndTopPillarWidth = gameResources->pillarPairs[right].topPillar->destRect.w;
		int rightEndBottomPillarXpos = gameResources->pillarPairs[right].bottomPillar->destRect.x;
		int rightEndBottomPillarWidth = gameResources->pillarPairs[right].bottomPillar->destRect.w;

		(gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x) += gameResources->pillarPairs[pillarPairIndex].topPillar->xTranslation;
		(gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x) += gameResources->pillarPairs[pillarPairIndex].bottomPillar->xTranslation;

		// If a pillar pair goes to the left of the screen (out of the window),place that pillar pair 
		// beside the pillar pair at the rightmost end of the screen
		if (gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x + gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.w < 0)
		{
			// Move the top pillar towards the left end of the screen
			gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x = rightEndTopPillarXpos + rightEndTopPillarWidth + gameResources->distanceBetweenPillars;
			
			// Add 10 points, if the pillar moves out of the window at the left end of the screen. This happens only if
			// the bird did not collide with any pillar till now. If collision happens, the game switches to the GAME_OVER state.
			gameResources->gameScore += 10;
			if (DEBUG) printf("[DEBUG INFO] Game score changed. Current score = %d\n", gameResources->gameScore);
		}
		if (gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x + gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.w < 0)
		{
			// Move the bottom pillar towards the left end of the screen
			gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x = rightEndBottomPillarXpos + rightEndBottomPillarWidth + gameResources->distanceBetweenPillars;
		}
		// Wraparound for the pillar pairs
		gameResources->pillarPairsLeftEndIndex = (left + 1) % gameResources->pillarPairCount;
		gameResources->pillarPairsRightEndIndex = (right + 1) % gameResources->pillarPairCount;

		// Copy the top and bottom pillars to the renderer
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
		// Check if the bottom pillar or the top pillar is colliding with the bird. If so, end the game
		if(CheckSpriteCollision(gameResources->pillarPairs[pillarPairIndex].topPillar, gameResources->bird) || 
			CheckSpriteCollision(gameResources->pillarPairs[pillarPairIndex].bottomPillar, gameResources->bird))
		{
			gameResources->gameState = GAME_OVER;
			return;
		}
	}

	return;
}

void HandleGameOver(SdlParameters* sdlParameters, GameResources* gameResources)
{
	// Display the backgrounds and do parallax effect on the background
	for (int backgroundSpriteIndex = 0; backgroundSpriteIndex < gameResources->backgroundSpriteCount; backgroundSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->backgroundSpriteArray[backgroundSpriteIndex]), sdlParameters, gameResources);
	}
	ParallaxEffect(gameResources->backgroundSpriteArray, gameResources->backgroundSpriteCount, &(gameResources->backgroundLeftEndIndex), &(gameResources->backgroundRightEndIndex));

	// Display 'Game Over' sprite
	CopySpriteToRenderer(gameResources->gameOver, sdlParameters, gameResources);

	// Copy the menu button to the renderer
	ResetGameResourceParameters(RESET_DEFAULT ,gameResources, sdlParameters);
	CopySpriteToRenderer(gameResources->menuButton, sdlParameters, gameResources);
	gameResources->gameState = GAME_OVER;
}

// Used to detect whether two sprites are colliding
static bool CheckSpriteCollision(Sprite* sprite1, Sprite* sprite2)
{
	// Check whether top right corner of B is within area of A
	if (sprite2->destRect.x + sprite2->destRect.w >= sprite1->destRect.x && sprite2->destRect.x <= sprite1->destRect.x + sprite1->destRect.w &&
	sprite2->destRect.y >= sprite1->destRect.y && sprite2->destRect.y <= sprite1->destRect.y + sprite1->destRect.h)
	{
	if (DEBUG) printf("[DEBUG INFO] Top right corner of %s colliding with %s\n", sprite2->name, sprite1->name);
	return true;
	}

	// Check whether bottom right corner of B is within area of A
	else if (sprite2->destRect.x + sprite2->destRect.w >= sprite1->destRect.x && sprite2->destRect.x + sprite2->destRect.w <= sprite1->destRect.x + sprite1->destRect.w &&
		sprite2->destRect.y + sprite2->destRect.h >= sprite1->destRect.y && sprite2->destRect.y + sprite2->destRect.h <= sprite1->destRect.y + sprite1->destRect.h)
	{
		if (DEBUG) printf("[DEBUG INFO] Bottom right corner of %s colliding with %s\n", sprite2->name, sprite1->name);
		return true;
	}

	// Check whether top left corner of B is within area of A
	else if (sprite2->destRect.x >= sprite1->destRect.x && sprite2->destRect.x <= sprite1->destRect.x + sprite1->destRect.w &&
		sprite2->destRect.y >= sprite1->destRect.y && sprite2->destRect.y <= sprite1->destRect.y + sprite1->destRect.h)
	{
		if (DEBUG) printf("[DEBUG INFO] Top left corner of %s colliding with %s\n", sprite2->name, sprite1->name);
		return true;
	}

	// Check whether bottom left corner of B is within area of A
	else if (sprite2->destRect.x >= sprite1->destRect.x && sprite2->destRect.x <= sprite1->destRect.x + sprite1->destRect.w &&
		sprite2->destRect.y + sprite2->destRect.h >= sprite1->destRect.y && sprite2->destRect.y + sprite2->destRect.h <= sprite1->destRect.y + sprite1->destRect.h)
	{
		if (DEBUG) printf("[DEBUG INFO] Bottom left corner of %s colliding with %s\n", sprite2->name, sprite1->name);
		return true;
	}

	return false;
}
