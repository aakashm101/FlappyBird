#include "LeaderboardController.h"
#include "ResourceController.h"

void handleLeaderboard(SdlParameters* sdlParameters, GameResources* gameResources)
{
	for (int backgroundSpriteIndex = 0; backgroundSpriteIndex < gameResources->backgroundSpriteCount; backgroundSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->backgroundSpriteArray[backgroundSpriteIndex]), sdlParameters, gameResources);
	}
	ParallaxEffect(gameResources->backgroundSpriteArray, gameResources->backgroundSpriteCount, &(gameResources->backgroundLeftEndIndex), &(gameResources->backgroundRightEndIndex));

	SDL_RenderCopy(
		sdlParameters->renderer,
		gameResources->comingSoonText->textTexture,
		NULL,
		&(gameResources->comingSoonText->destRect));

	return;
}
