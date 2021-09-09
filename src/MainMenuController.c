#include <stdio.h>
#include "MainMenuController.h"
#include "SDLController.h"
#include "ResourceController.h"

void showMainMenu(GameResources* gameResources, SdlParameters* sdlParameters)
{
	if (!gameResources)
	{
		printf("[ERROR] Cannot display main menu with empty game resources!\n");
		return;
	}
	for (int backgroundSpriteIndex = 0; backgroundSpriteIndex < gameResources->backgroundSpriteCount; backgroundSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->backgroundSpriteArray[backgroundSpriteIndex]), sdlParameters, gameResources);
	}
	CopySpriteToRenderer(gameResources->flappyBirdLogo, sdlParameters, gameResources);
	CopySpriteToRenderer(gameResources->playButton, sdlParameters, gameResources);
	CopySpriteToRenderer(gameResources->leaderboardButton, sdlParameters, gameResources);
	ParallaxEffect(gameResources->backgroundSpriteArray, gameResources->backgroundSpriteCount, &(gameResources->backgroundLeftEndIndex), &(gameResources->backgroundRightEndIndex));

	return;
}