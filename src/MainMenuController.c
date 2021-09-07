#include "MainMenuController.h"
#include "SDLController.h"

void showMainMenu(GameResources* gameResources, SdlParameters* sdlParameters)
{
	for (int backgroundSpriteIndex = 0; backgroundSpriteIndex < gameResources->backgroundSpriteCount; backgroundSpriteIndex++)
	{
		CopySpriteToRenderer(&(gameResources->backgroundSpriteArray[backgroundSpriteIndex]), sdlParameters, gameResources);
	}
	CopySpriteToRenderer(gameResources->flappyBirdLogo, sdlParameters, gameResources);
	CopySpriteToRenderer(gameResources->playButton, sdlParameters, gameResources);
	CopySpriteToRenderer(gameResources->leaderboardButton, sdlParameters, gameResources);
	return;
}