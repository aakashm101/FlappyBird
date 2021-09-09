#include <stdio.h>
#include <SDL2/SDL.h>
#include "MouseController.h"
#include "GameplayController.h"
#include "LeaderboardController.h"

void HandleMouseEvents(SdlParameters* sdlParameters, GameResources* gameResources)
{
	int x, y;
	x = 0;
	y = 0;

	// Get the coordinates on the screen where the mouse click occurred
	if (sdlParameters->event.button.button == SDL_BUTTON_LEFT ||
		sdlParameters->event.button.button == SDL_BUTTON_MIDDLE ||
		sdlParameters->event.button.button == SDL_BUTTON_RIGHT)
	{
		SDL_GetMouseState(&x, &y);
		if (DEBUG) printf("[DEBUG INFO] Mouse button pressed. X = %d, Y = %d.\n", x, y);
	}

	// If the user is in the main menu
	if (gameResources->gameState == GAME_MAIN_MENU)
	{
		if (clickedOnSprite(gameResources->playButton, x, y))	// If user clicks on play button, start the game
		{
			if (DEBUG) printf("[DEBUG INFO] Mouse clicked on play button!\n");
			gameResources->gameState = GAME_RUNNING;
		}
		else if (clickedOnSprite(gameResources->leaderboardButton, x, y))	// If user clicks on leaderboard button, display the scores
		{
			if (DEBUG) printf("[DEBUG INFO] Mouse clicked on leaderboards button!\n");
			gameResources->gameState = GAME_LEADERBOARD;
		}
	}

	return;
}

// Used to check is the mouse clicked within the destRect of the sprite
bool clickedOnSprite(const Sprite* const sprite, int mouseX, int mouseY)
{
	if (mouseX >= sprite->destRect.x && mouseX <= sprite->destRect.x + sprite->destRect.w &&
		mouseY >= sprite->destRect.y && mouseY <= sprite->destRect.y + sprite->destRect.h)
	{
		return true;
	}
	return false;
}
