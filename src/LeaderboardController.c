#include "LeaderboardController.h"
#include "SDLController.h"
#include "ResourceController.h"

void HandleLeaderboard(SdlParameters* sdlParameters, GameResources* gameResources)
{
	// Copy the background sprites to renderer and apply parallax background effect
	for (int backgroundSpriteIndex = 0; backgroundSpriteIndex < gameResources->backgroundSpriteCount; backgroundSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->backgroundSpriteArray[backgroundSpriteIndex]), sdlParameters, gameResources);
	}
	ParallaxEffect(gameResources->backgroundSpriteArray, gameResources->backgroundSpriteCount, &(gameResources->backgroundLeftEndIndex), &(gameResources->backgroundRightEndIndex));

	// Copy the 'Coming Soon' text to the renderer
	SDL_RenderCopy(sdlParameters->renderer, gameResources->comingSoonText->textTexture, NULL, &(gameResources->comingSoonText->destRect));

	// Copy the menu button to the renderer
	CopySpriteToRenderer(gameResources->menuButton, sdlParameters, gameResources);
	return;
}
