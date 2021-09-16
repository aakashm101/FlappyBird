#include <stdio.h>
#include <SDL2/SDL.h>
#include "MouseController.h"
#include "GameplayController.h"
#include "LeaderboardController.h"

void HandleMouseEvents(SdlParameters* sdlParameters, GameResources* gameResources)
{
	// Get the coordinates on the screen where the mouse click occurred
	if (sdlParameters->event.button.button == SDL_BUTTON_LEFT ||
		sdlParameters->event.button.button == SDL_BUTTON_MIDDLE ||
		sdlParameters->event.button.button == SDL_BUTTON_RIGHT)
	{
		int x, y;

		SDL_GetMouseState(&x, &y);
		if (DEBUG) printf("[DEBUG INFO] Mouse button pressed. X = %d, Y = %d.\n", x, y);

		// If the user is in the main menu
		if (gameResources->gameState == GAME_MAIN_MENU)
		{
			if (ClickedOnSprite(gameResources->playButton, x, y))	// If user clicks on play button, start the game
			{
				if (DEBUG) printf("[DEBUG INFO] Mouse clicked on %s!\n", gameResources->playButton->name);
				gameResources->gameState = GAME_RUNNING;
				return;
			}
			else if (ClickedOnSprite(gameResources->leaderboardButton, x, y))	// If user clicks on leaderboard button, display the scores
			{
				if (DEBUG) printf("[DEBUG INFO] Mouse clicked on %s!\n", gameResources->leaderboardButton->name);
				gameResources->gameState = GAME_LEADERBOARD;
				return;
			}
		}
		// If the user is viewing the leaderboards
		else if (gameResources->gameState == GAME_LEADERBOARD)
		{
			if (ClickedOnSprite(gameResources->menuButton, x, y))
			{
				if (DEBUG) printf("[DEBUG INFO] Mouse clicked on %s!\n", gameResources->menuButton->name);
				gameResources->gameState = GAME_MAIN_MENU;
				return;
			}
		}
		// If the user is viewing the current game score after the game is over
		else if (gameResources->gameState == GAME_OVER)
		{
			if (ClickedOnSprite(gameResources->menuButton, x, y))
			{
				if (DEBUG) printf("[DEBUG INFO] Mouse clicked on %s!\n", gameResources->menuButton->name);
				gameResources->gameState = GAME_MAIN_MENU;
				return;
			}
		}
	}

	return;
}

// Used to check whether mouse button is clicked within the destRect of the sprite
bool ClickedOnSprite(const Sprite* const sprite, int mouseX, int mouseY)
{
	if (mouseX >= sprite->destRect.x && mouseX <= sprite->destRect.x + sprite->destRect.w &&
		mouseY >= sprite->destRect.y && mouseY <= sprite->destRect.y + sprite->destRect.h)
	{
		return true;
	}
	return false;
}
