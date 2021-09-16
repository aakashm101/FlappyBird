#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_image.h>
#include "global.h"
#include "ResourceController.h"

int LoadGameResources(GameResources* gameResources, const SdlParameters* const sdlParameters)
{
	// Fetch the window dimension values
	const int WINDOW_WIDTH = sdlParameters->dm.w;
	const int WINDOW_HEIGHT = sdlParameters->dm.h;

	const SDL_Color fontColor = { 0xFF, 0xFF, 0xFF, 0xFF };

	// Background width and height (Obtained from tilemap)
	const int BACKGROUND_WIDTH = 144;
	const int BACKGROUND_HEIGHT = 256;
	
	// Floor X,Y coordinates, width and height (Obtained from tilemap)
	const int FLOOR_XPOS = 292;
	const int FLOOR_YPOS = 0;
	const int FLOOR_WIDTH = 68;
	const int FLOOR_HEIGHT = 56;

	// Scale the background image to the window size without affecting dimensions of floor image
	const double BACKGROUND_WIDTH_HEIGHT_RATIO = (double)BACKGROUND_WIDTH / BACKGROUND_HEIGHT;
	const int SCALED_BACKGROUND_HEIGHT = (sdlParameters->dm.h);
	const int SCALED_BACKGROUND_WIDTH = ceil((double)SCALED_BACKGROUND_HEIGHT * BACKGROUND_WIDTH_HEIGHT_RATIO);

	// Scale the floor image to the window height / 5 without affecting dimensions of the floor image
	const double FLOOR_WIDTH_HEIGHT_RATIO = (double)FLOOR_WIDTH / FLOOR_HEIGHT;
	const int SCALED_FLOOR_HEIGHT = ceil((double)WINDOW_HEIGHT / 5);
	const int SCALED_FLOOR_WIDTH = ceil((double)SCALED_FLOOR_HEIGHT * FLOOR_WIDTH_HEIGHT_RATIO);

	// Open the main menu at the start
	gameResources->gameState = GAME_MAIN_MENU;

	// Load the tilemap from the disk
	gameResources->tileMapPath = "res/tilemap.png";
	gameResources->tileMap = IMG_LoadTexture(sdlParameters->renderer, gameResources->tileMapPath);
	if (!gameResources->tileMap)
	{
		printf("[ERROR] TileMap %s failed to load!\n", gameResources->tileMapPath);
		return -1;
	}
	SDL_QueryTexture(gameResources->tileMap, NULL, NULL, &(gameResources->tileMapWidth), &(gameResources->tileMapHeight));
	if (DEBUG) printf("[DEBUG INFO] Tilemap %s loaded. Width = %d, Height = %d.\n", gameResources->tileMapPath, gameResources->tileMapWidth, gameResources->tileMapHeight);

	// Used to store how many background images will fit inside the window (Using multiple scrolling background images to create parallax effect)
	// Use one more background image (It can be partially outside the current window) to create the parallax effect
	gameResources->backgroundSpriteCount = ceil((double)WINDOW_WIDTH / SCALED_BACKGROUND_WIDTH);
	gameResources->backgroundSpriteCount = gameResources->backgroundSpriteCount + 1;
	gameResources->backgroundLeftEndIndex = 0;
	gameResources->backgroundRightEndIndex = gameResources->backgroundSpriteCount - 1;
	gameResources->backgroundSpriteArray = (Sprite*)calloc(gameResources->backgroundSpriteCount, sizeof(Sprite));	// Allocate memory for background sprites
	if (!gameResources->backgroundSpriteArray)
	{
		printf("[ERROR] Failed to allocate memory for backround sprite!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] Sprite '%s' created. Count = %d\n", "background", gameResources->backgroundSpriteCount);

	// Used to store how many floor images will fit inside the window (Using multiple scrolling floor images to create parallax effect)
	// Use one more floor image (It can be partially outside the current window) to create the parallax effect
	gameResources->floorSpriteCount = ceil((double)WINDOW_WIDTH / SCALED_FLOOR_WIDTH);
	gameResources->floorSpriteCount = gameResources->floorSpriteCount + 1;
	gameResources->floorLeftEndIndex = 0;
	gameResources->floorRightEndIndex = gameResources->floorSpriteCount - 1;
	gameResources->floorSpriteArray = (Sprite*)calloc(gameResources->floorSpriteCount, sizeof(Sprite));		// Allocate memory for floor sprites
	if (!gameResources->floorSpriteArray)
	{
		printf("[ERROR] Failed to allocate memory for floor sprite!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] Sprite '%s' created. Count = %d\n", "floor", gameResources->floorSpriteCount);

	// Allocate memory for bird sprite
	gameResources->flappyBirdLogo = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->flappyBirdLogo)
	{
		printf("[ERROR] Memory allocation failed for flappy bird logo!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] Sprite '%s' created.\n", "bird");

	// Allocate memory for play button sprite
	gameResources->playButton = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->playButton)
	{
		printf("[ERROR] Memory allocation failed for play button!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] '%s' sprite created.\n", "play_button");

	// Allocate memory for leaderboard button sprite
	gameResources->leaderboardButton = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->leaderboardButton)
	{
		printf("[ERROR] Memory allocation failed for leaderboard button!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] '%s' sprite created.\n", "leaderboard_button");

	// Allocate memory for menu button sprite
	gameResources->menuButton = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->menuButton)
	{
		printf("[ERROR] Memory allocation failed for menu button!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] '%s' sprite created.\n", "menu_button");

	// Allocate memory for get_ready sprite
	gameResources->getReady = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->getReady)
	{
		printf("[ERROR] Memory allocation failed for sprite containing 'Get Ready' text!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] '%s' sprite created.\n", "get_ready_button");

	// Allocate memory for game_over sprite
	gameResources->gameOver = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->gameOver)
	{
		printf("[ERROR] Memory allocation failed for sprite containing 'Game Over' text!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] '%s' sprite created.\n", "game_over_button");

	// Allocate memory for bird sprite
	gameResources->bird = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->bird)
	{
		printf("[ERROR] Memory allocation failed for bird sprite!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] '%s' sprite created.\n", "bird");

	// Allocate memory for 'Coming soon' Text
	gameResources->comingSoonText = CreateText("Coming Soon", 40, sdlParameters, fontColor);
	if (!gameResources->comingSoonText)
	{
		printf("[ERROR] Text creation failed!\n");
		return -1;
	}
	if (DEBUG) printf("[DEBUG INFO] '%s' sprite created.\n", "coming_soon");

	// Allocate memory for big number sprites (0-9)
	gameResources->bigNumberSpriteArray = (Sprite**)calloc(10, sizeof(Sprite*));
	if (!gameResources->bigNumberSpriteArray)
	{
		printf("[ERROR] Big number sprite array creation failed!\n");
		return -1;
	}
	for (int bigNumberSpriteIndex = 0; bigNumberSpriteIndex < 10; bigNumberSpriteIndex++)
	{
		gameResources->bigNumberSpriteArray[bigNumberSpriteIndex] = (Sprite*)malloc(sizeof(Sprite));
		if (!gameResources->bigNumberSpriteArray[bigNumberSpriteIndex])
		{
			printf("[ERROR] Big number %d sprite array element creation failed!\n", bigNumberSpriteIndex);
			return -1;	// Need to add code to deallocate memory for previous elements if the current element memory allocation fails
		}
	}
	if (DEBUG) printf("[DEBUG INFO] '%s' sprite created.\n", "big_number_array");

	// Reset the game resources (Need to reset the sprite parameters in game resources before creating pillar pairs)
	ResetGameResourceParameters(RESET_BACKGROUND | RESET_FLOOR, gameResources, sdlParameters);

	// Set the values for create pillar pairs and create pillar pairs
	gameResources->pillarPairMinimumSpacing = 3 * (gameResources->bird->destRect.h);
	gameResources->pillarPairCount = ceil((double)sdlParameters->dm.w / 400);
	gameResources->pillarPairsLeftEndIndex = 0;
	gameResources->pillarPairsRightEndIndex = gameResources->pillarPairCount - 1;
	gameResources->distanceBetweenPillars = ceil((double)WINDOW_WIDTH / 5);
	gameResources->pillarPairs = CreatePillarPairs(gameResources, sdlParameters, gameResources->pillarPairCount);
	if (!gameResources->pillarPairs)
	{
		return -1;
	}

	return 0;
}

// Used to reset the values of the sprites to default values (Useful for restarting the game)
void ResetGameResourceParameters(ResetParameters resetParameters, GameResources* gameResources, SdlParameters* sdlParameters)
{
	if (!gameResources)
	{
		printf("[ERROR] Cannot reset game resource parameters for empty game resources!\n");
		return;
	}

	// Fetch the window dimension values
	const int WINDOW_WIDTH = sdlParameters->dm.w;
	const int WINDOW_HEIGHT = sdlParameters->dm.h;

	// Background X,Y coordinates, width and height (Obtained from tilemap)
	const int BACKGROUND_WIDTH = 144;
	const int BACKGROUND_HEIGHT = 256;

	// Floor X,Y coordinates, width and height (Obtained from tilemap)
	const int FLOOR_XPOS = 292;
	const int FLOOR_YPOS = 0;
	const int FLOOR_WIDTH = 68;
	const int FLOOR_HEIGHT = 56;

	// Flappy bird logo X,Y coordinates, width and height (Obtained from tilemap)
	const int FLAPPY_BIRD_LOGO_XPOS = 350;
	const int FLAPPY_BIRD_LOGO_YPOS = 90;
	const int FLAPPY_BIRD_LOGO_WIDTH = 92;
	const int FLAPPY_BIRD_LOGO_HEIGHT = 26;

	// Play button X,Y coordinates, width and height (Obtained from tilemap)
	const int PLAY_BUTTON_XPOS = 354;
	const int PLAY_BUTTON_YPOS = 118;
	const int PLAY_BUTTON_WIDTH = 52;
	const int PLAY_BUTTON_HEIGHT = 30;

	// Leaderboard button X,Y coordinates, width and height (Obtained from tilemap)
	const int LEADERBOARD_BUTTON_XPOS = 414;
	const int LEADERBOARD_BUTTON_YPOS = 118;
	const int LEADERBOARD_BUTTON_WIDTH = 52;
	const int LEADERBOARD_BUTTON_HEIGHT = 30;

	// Menu button X,Y coordinates, width and height (Obtained from tilemap)
	const int MENU_BUTTON_XPOS = 462;
	const int MENU_BUTTON_YPOS = 26;
	const int MENU_BUTTON_WIDTH = 40;
	const int MENU_BUTTON_HEIGHT = 14;

	// Bird X,Y coordinates, width and height (Obtained from tilemap)
	const int BIRD_XPOS = 2;
	const int BIRD_YPOS = 490;
	const int BIRD_WIDTH = 18;
	const int BIRD_HEIGHT = 14;

	// Big number X,Y coordinates, width and height (Obtained from tilemap)
	const int BIG_NUMBER0_XPOS = 496; const int BIG_NUMBER0_YPOS = 60; const int BIG_NUMBER0_WIDTH = 12; const int BIG_NUMBER0_HEIGHT = 19;
	const int BIG_NUMBER1_XPOS = 136; const int BIG_NUMBER1_YPOS = 455; const int BIG_NUMBER1_WIDTH = 8; const int BIG_NUMBER1_HEIGHT = 19;
	const int BIG_NUMBER2_XPOS = 292; const int BIG_NUMBER2_YPOS = 160; const int BIG_NUMBER2_WIDTH = 12; const int BIG_NUMBER2_HEIGHT = 19;
	const int BIG_NUMBER3_XPOS = 306; const int BIG_NUMBER3_YPOS = 160; const int BIG_NUMBER3_WIDTH = 12; const int BIG_NUMBER3_HEIGHT = 19;
	const int BIG_NUMBER4_XPOS = 320; const int BIG_NUMBER4_YPOS = 160; const int BIG_NUMBER4_WIDTH = 12; const int BIG_NUMBER4_HEIGHT = 19;
	const int BIG_NUMBER5_XPOS = 334; const int BIG_NUMBER5_YPOS = 160; const int BIG_NUMBER5_WIDTH = 12; const int BIG_NUMBER5_HEIGHT = 19;
	const int BIG_NUMBER6_XPOS = 292; const int BIG_NUMBER6_YPOS = 184; const int BIG_NUMBER6_WIDTH = 12; const int BIG_NUMBER6_HEIGHT = 19;
	const int BIG_NUMBER7_XPOS = 306; const int BIG_NUMBER7_YPOS = 184; const int BIG_NUMBER7_WIDTH = 12; const int BIG_NUMBER7_HEIGHT = 19;
	const int BIG_NUMBER8_XPOS = 320; const int BIG_NUMBER8_YPOS = 184; const int BIG_NUMBER8_WIDTH = 12; const int BIG_NUMBER8_HEIGHT = 19;
	const int BIG_NUMBER9_XPOS = 334; const int BIG_NUMBER9_YPOS = 184; const int BIG_NUMBER9_WIDTH = 12; const int BIG_NUMBER9_HEIGHT = 19;

	// Scale the background image to the window size without affecting dimensions of floor image
	const float BACKGROUND_WIDTH_HEIGHT_RATIO = (float)BACKGROUND_WIDTH / BACKGROUND_HEIGHT;
	const int SCALED_BACKGROUND_HEIGHT = (sdlParameters->dm.h);
	const int SCALED_BACKGROUND_WIDTH = ceil((double)SCALED_BACKGROUND_HEIGHT * BACKGROUND_WIDTH_HEIGHT_RATIO);

	// Scale the floor image to the window height / 5 without affecting dimensions of the floor image
	const float FLOOR_WIDTH_HEIGHT_RATIO = (float)FLOOR_WIDTH / FLOOR_HEIGHT;
	const int SCALED_FLOOR_HEIGHT = ceil((double)WINDOW_HEIGHT / 5);
	const int SCALED_FLOOR_WIDTH = ceil((double)SCALED_FLOOR_HEIGHT * FLOOR_WIDTH_HEIGHT_RATIO);

	// Set the global xTranslation value for parallax effect (Can be overridden)
	gameResources->parallaxGlobalXTranslation = -2;

	// Set the game score
	gameResources->gameScore = 0;
	
	// Set the default values for background sprites
	for (int backgroundSpriteIndex = 0; backgroundSpriteIndex < gameResources->backgroundSpriteCount; backgroundSpriteIndex++)
	{
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].name = "background";
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].angle = 0;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].xTranslation = gameResources->parallaxGlobalXTranslation;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].yTranslation = 0;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].gravity = 0;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].maxGravity = 0;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].lift = 0;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].maxLift = 0;
		if (resetParameters == RESET_BACKGROUND || (resetParameters == (RESET_BACKGROUND | RESET_FLOOR)))
		{
			gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.x = (backgroundSpriteIndex == 0) ? 0 : gameResources->backgroundSpriteArray[backgroundSpriteIndex - 1].destRect.x + gameResources->backgroundSpriteArray[backgroundSpriteIndex - 1].destRect.w;	// Place the backgrounds size by side
		}
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].srcRect.y = 0;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].srcRect.w = BACKGROUND_WIDTH;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].srcRect.h = BACKGROUND_HEIGHT;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.w = BACKGROUND_WIDTH;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.h = BACKGROUND_HEIGHT;
		ScaleSpriteToFitOnArea(
			&(gameResources->backgroundSpriteArray[backgroundSpriteIndex]),
			gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.x,
			gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.y,
			SCALED_BACKGROUND_WIDTH,
			SCALED_BACKGROUND_HEIGHT);
	}
	if (DEBUG) printf("[DEBUG INFO] Sprite '%s' parameters reset\n", "background");

	// Set the default values for floor sprites
	for (int floorSpriteIndex = 0; floorSpriteIndex < gameResources->floorSpriteCount; floorSpriteIndex++)
	{
		gameResources->floorSpriteArray[floorSpriteIndex].name = "floor";
		gameResources->floorSpriteArray[floorSpriteIndex].angle = 0;
		gameResources->floorSpriteArray[floorSpriteIndex].xTranslation = gameResources->parallaxGlobalXTranslation;
		gameResources->floorSpriteArray[floorSpriteIndex].yTranslation = 0;
		gameResources->floorSpriteArray[floorSpriteIndex].gravity = 0;
		gameResources->floorSpriteArray[floorSpriteIndex].maxGravity = 0;
		gameResources->floorSpriteArray[floorSpriteIndex].lift = 0;
		gameResources->floorSpriteArray[floorSpriteIndex].maxLift = 0;
		gameResources->floorSpriteArray[floorSpriteIndex].srcRect.x = FLOOR_XPOS;
		gameResources->floorSpriteArray[floorSpriteIndex].srcRect.y = FLOOR_YPOS;
		gameResources->floorSpriteArray[floorSpriteIndex].srcRect.w = FLOOR_WIDTH;
		gameResources->floorSpriteArray[floorSpriteIndex].srcRect.h = FLOOR_HEIGHT;
		if (resetParameters == RESET_FLOOR || (resetParameters == (RESET_BACKGROUND | RESET_FLOOR)))
		{
			gameResources->floorSpriteArray[floorSpriteIndex].destRect.x = (floorSpriteIndex == 0) ? 0 : gameResources->floorSpriteArray[floorSpriteIndex - 1].destRect.x + gameResources->floorSpriteArray[floorSpriteIndex - 1].destRect.w;	// Place the floors size by side
		}
		gameResources->floorSpriteArray[floorSpriteIndex].destRect.y = WINDOW_HEIGHT - ((float)WINDOW_HEIGHT / 5);
		gameResources->floorSpriteArray[floorSpriteIndex].destRect.w = SCALED_FLOOR_WIDTH;
		gameResources->floorSpriteArray[floorSpriteIndex].destRect.h = SCALED_FLOOR_HEIGHT;
		ScaleSpriteToFitOnArea(
			&(gameResources->floorSpriteArray[floorSpriteIndex]),
			gameResources->floorSpriteArray[floorSpriteIndex].destRect.x,
			gameResources->floorSpriteArray[floorSpriteIndex].destRect.y,
			SCALED_FLOOR_WIDTH,
			SCALED_FLOOR_HEIGHT);
	}
	if (DEBUG) printf("[DEBUG INFO] Sprite '%s' parameters reset\n", "floor");

	// Set the default values for flappy_bird_logo sprite
	// Fit the flappy bird logo to the top 1/3 of the area of the screen
	gameResources->flappyBirdLogo->name = "flappy_bird_logo";
	gameResources->flappyBirdLogo->angle = 0;
	gameResources->flappyBirdLogo->xTranslation = 0;
	gameResources->flappyBirdLogo->yTranslation = 0;
	gameResources->flappyBirdLogo->gravity = 0;
	gameResources->flappyBirdLogo->maxGravity = 0;
	gameResources->flappyBirdLogo->lift = 0;
	gameResources->flappyBirdLogo->maxLift = 0;
	gameResources->flappyBirdLogo->srcRect.x = FLAPPY_BIRD_LOGO_XPOS;
	gameResources->flappyBirdLogo->srcRect.y = FLAPPY_BIRD_LOGO_YPOS;
	gameResources->flappyBirdLogo->srcRect.w = FLAPPY_BIRD_LOGO_WIDTH;
	gameResources->flappyBirdLogo->srcRect.h = FLAPPY_BIRD_LOGO_HEIGHT;
	gameResources->flappyBirdLogo->destRect.x = 0;
	gameResources->flappyBirdLogo->destRect.y = 0;
	gameResources->flappyBirdLogo->destRect.w = FLAPPY_BIRD_LOGO_WIDTH;
	gameResources->flappyBirdLogo->destRect.h = FLAPPY_BIRD_LOGO_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->flappyBirdLogo, 0, 0, WINDOW_WIDTH, ceil((double)WINDOW_HEIGHT / 3));
	CenterSpriteHorizontallyOnScreen(gameResources->flappyBirdLogo, sdlParameters);
	ScaleSpriteInPlaceByFactor(gameResources->flappyBirdLogo, 0.8);
	if (DEBUG) printf("[DEBUG INFO] Sprite '%s' parameters reset\n", gameResources->flappyBirdLogo->name);

	// Set the default values for play button sprite
	gameResources->playButton->name = "play_button";
	gameResources->playButton->angle = 0;
	gameResources->playButton->xTranslation = 0;
	gameResources->playButton->yTranslation = 0;
	gameResources->playButton->gravity = 0;
	gameResources->playButton->maxGravity = 0;
	gameResources->playButton->lift = 0;
	gameResources->playButton->maxLift = 0;
	gameResources->playButton->srcRect.x = PLAY_BUTTON_XPOS;
	gameResources->playButton->srcRect.y = PLAY_BUTTON_YPOS;
	gameResources->playButton->srcRect.w = PLAY_BUTTON_WIDTH;
	gameResources->playButton->srcRect.h = PLAY_BUTTON_HEIGHT;
	gameResources->playButton->destRect.x = 0;
	gameResources->playButton->destRect.y = (float)WINDOW_HEIGHT / 2 + PLAY_BUTTON_HEIGHT;
	gameResources->playButton->destRect.w = (float)WINDOW_WIDTH / 2;
	gameResources->playButton->destRect.h = (float)WINDOW_HEIGHT / 2;
	ScaleSpriteInPlaceByFactor(gameResources->playButton, 0.5);
	if (DEBUG) printf("[DEBUG INFO] Sprite '%s' parameters reset\n", gameResources->playButton->name);

	// Set the default values for leaderboard button sprite
	gameResources->leaderboardButton->name = "leaderboard_button";
	gameResources->leaderboardButton->angle = 0;
	gameResources->leaderboardButton->xTranslation = 0;
	gameResources->leaderboardButton->yTranslation = 0;
	gameResources->leaderboardButton->gravity = 0;
	gameResources->leaderboardButton->maxGravity = 0;
	gameResources->leaderboardButton->lift = 0;
	gameResources->leaderboardButton->maxLift = 0;
	gameResources->leaderboardButton->srcRect.x = LEADERBOARD_BUTTON_XPOS;
	gameResources->leaderboardButton->srcRect.y = LEADERBOARD_BUTTON_YPOS;
	gameResources->leaderboardButton->srcRect.w = LEADERBOARD_BUTTON_WIDTH;
	gameResources->leaderboardButton->srcRect.h = LEADERBOARD_BUTTON_HEIGHT;
	gameResources->leaderboardButton->destRect.x = (float)WINDOW_WIDTH / 2;
	gameResources->leaderboardButton->destRect.y = (float)WINDOW_HEIGHT / 2 + LEADERBOARD_BUTTON_HEIGHT;
	gameResources->leaderboardButton->destRect.w = (float)WINDOW_WIDTH / 2;
	gameResources->leaderboardButton->destRect.h = (float)WINDOW_HEIGHT / 2;
	ScaleSpriteInPlaceByFactor(gameResources->leaderboardButton, 0.5);
	if (DEBUG) printf("[DEBUG INFO] Sprite '%s' parameters reset\n", gameResources->leaderboardButton->name);

	// Set the default values for menu button sprite
	gameResources->menuButton->name = "menu_button";
	gameResources->menuButton->angle = 0;
	gameResources->menuButton->xTranslation = 0;
	gameResources->menuButton->yTranslation = 0;
	gameResources->menuButton->gravity = 0;
	gameResources->menuButton->maxGravity = 0;
	gameResources->menuButton->lift = 0;
	gameResources->menuButton->maxLift = 0;
	gameResources->menuButton->srcRect.x = MENU_BUTTON_XPOS;
	gameResources->menuButton->srcRect.y = MENU_BUTTON_YPOS;
	gameResources->menuButton->srcRect.w = MENU_BUTTON_WIDTH;
	gameResources->menuButton->srcRect.h = MENU_BUTTON_HEIGHT;
	gameResources->menuButton->destRect.x = 0;
	gameResources->menuButton->destRect.y = 0;
	gameResources->menuButton->destRect.w = MENU_BUTTON_WIDTH;
	gameResources->menuButton->destRect.h = MENU_BUTTON_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->menuButton, 0, 0, (float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 6);
	ScaleSpriteInPlaceByFactor(gameResources->menuButton, 0.3);
	gameResources->menuButton->destRect.y = WINDOW_HEIGHT - (2 * gameResources->menuButton->destRect.h);	// Place the button at the bottom
	CenterSpriteHorizontallyOnScreen(gameResources->menuButton, sdlParameters);
	if (DEBUG) printf("[DEBUG INFO] Sprite '%s' parameters reset\n", gameResources->menuButton->name);

	// Set the srcRect and destRect and default values for sprite containing 'Get Ready' text from the tilemap
	// Place it at WINDOW HEIGHT/6 distance from the top and scale the texture to fit
	gameResources->getReady->name = "get_ready";
	gameResources->getReady->angle = 0;
	gameResources->getReady->xTranslation = 0;
	gameResources->getReady->yTranslation = 0;
	gameResources->getReady->gravity = 0;
	gameResources->getReady->maxGravity = 0;
	gameResources->getReady->lift = 0;
	gameResources->getReady->maxLift = 0;
	gameResources->getReady->srcRect.x = 294;
	gameResources->getReady->srcRect.y = 58;
	gameResources->getReady->srcRect.w = 94;
	gameResources->getReady->srcRect.h = 28;
	gameResources->getReady->destRect.x = 0;
	gameResources->getReady->destRect.y = ceil((double)WINDOW_HEIGHT / 6);
	gameResources->getReady->destRect.w = gameResources->getReady->srcRect.w;
	gameResources->getReady->destRect.h = gameResources->getReady->srcRect.h;
	ScaleSpriteToFitOnArea(gameResources->getReady, 0, 0, WINDOW_WIDTH, (float)WINDOW_HEIGHT / 6);
	CenterSpriteHorizontallyOnScreen(gameResources->getReady, sdlParameters);
	ScaleSpriteInPlaceByFactor(gameResources->getReady, 0.5);
	if (DEBUG) printf("[DEBUG INFO] Sprite '%s' parameters reset\n", gameResources->getReady->name);

	// Set the srcRect and destRect and default values for sprite containing 'Game Over' text from the tilemap
	// Place it at WINDOW HEIGHT/6 distance from the top and scale the texture to fit
	gameResources->gameOver->name = "game_over";
	gameResources->gameOver->angle = 0;
	gameResources->gameOver->xTranslation = 0;
	gameResources->gameOver->yTranslation = 0;
	gameResources->gameOver->gravity = 0;
	gameResources->gameOver->maxGravity = 0;
	gameResources->gameOver->lift = 0;
	gameResources->gameOver->maxLift = 0;
	gameResources->gameOver->srcRect.x = 394;
	gameResources->gameOver->srcRect.y = 58;
	gameResources->gameOver->srcRect.w = 98;
	gameResources->gameOver->srcRect.h = 28;
	gameResources->gameOver->destRect.x = 0;
	gameResources->gameOver->destRect.y = ceil((double)WINDOW_HEIGHT / 6);
	gameResources->gameOver->destRect.w = gameResources->gameOver->srcRect.w;
	gameResources->gameOver->destRect.h = gameResources->gameOver->srcRect.h;
	ScaleSpriteToFitOnArea(gameResources->gameOver, 0, 0, WINDOW_WIDTH, (float)WINDOW_HEIGHT / 6);
	CenterSpriteHorizontallyOnScreen(gameResources->gameOver, sdlParameters);
	ScaleSpriteInPlaceByFactor(gameResources->gameOver, 0.5);
	if (DEBUG) printf("[DEBUG INFO] Sprite '%s' parameters reset\n", gameResources->gameOver->name);

	// Set the default values for bird sprite
	gameResources->bird->name = "bird";
	gameResources->bird->angle = 0;
	gameResources->bird->xTranslation = 0;
	gameResources->bird->yTranslation = 0;
	gameResources->bird->gravity = 1;
	gameResources->bird->maxGravity = sdlParameters->dm.h / 190.0;
	gameResources->bird->lift = 1;
	gameResources->bird->maxLift = 1;
	gameResources->bird->srcRect.x = BIRD_XPOS;
	gameResources->bird->srcRect.y = BIRD_YPOS;
	gameResources->bird->srcRect.w = BIRD_WIDTH;
	gameResources->bird->srcRect.h = BIRD_HEIGHT;
	gameResources->bird->destRect.x = 0;
	gameResources->bird->destRect.y = 0;
	gameResources->bird->destRect.w = BIRD_WIDTH * (WINDOW_HEIGHT / 200);	// Dynamically scale the bird sprite based on window height
	gameResources->bird->destRect.h = BIRD_HEIGHT * (WINDOW_HEIGHT / 200);	// Dynamically scale the bird sprite based on window height
	CenterSpriteVerticallyOnScreen(gameResources->bird, sdlParameters);
	if (DEBUG) printf("[DEBUG INFO] Sprite '%s' parameters reset\n", gameResources->bird->name);

	// Reset pillar pair parameters
	for (int pillarPairIndex = 0; pillarPairIndex < gameResources->pillarPairCount; pillarPairIndex++)
	{
		SetRandomPillarHeight(gameResources->pillarPairs, gameResources, sdlParameters);
		
		// Set pillar pairs equidistant from each other
		gameResources->pillarPairs[pillarPairIndex].topPillar->destRect.x = (pillarPairIndex == 0) ? (gameResources->distanceBetweenPillars) : (gameResources->pillarPairs[pillarPairIndex - 1].topPillar->destRect.x + gameResources->pillarPairs[pillarPairIndex - 1].topPillar->destRect.w + gameResources->distanceBetweenPillars);
		gameResources->pillarPairs[pillarPairIndex].bottomPillar->destRect.x = (pillarPairIndex == 0) ? (gameResources->distanceBetweenPillars) : (gameResources->pillarPairs[pillarPairIndex - 1].bottomPillar->destRect.x + gameResources->pillarPairs[pillarPairIndex - 1].bottomPillar->destRect.w + gameResources->distanceBetweenPillars);
	}

	// Reset big number sprite array parameters
	gameResources->bigNumberSpriteArray[0]->name = "big_number_0";
	gameResources->bigNumberSpriteArray[0]->xTranslation = 0;
	gameResources->bigNumberSpriteArray[0]->yTranslation = 0;
	gameResources->bigNumberSpriteArray[0]->angle = 0;
	gameResources->bigNumberSpriteArray[0]->gravity = 0;
	gameResources->bigNumberSpriteArray[0]->maxGravity = 0;
	gameResources->bigNumberSpriteArray[0]->lift = 0;
	gameResources->bigNumberSpriteArray[0]->maxLift = 0;
	gameResources->bigNumberSpriteArray[0]->srcRect.x = BIG_NUMBER0_XPOS;
	gameResources->bigNumberSpriteArray[0]->srcRect.y = BIG_NUMBER0_YPOS;
	gameResources->bigNumberSpriteArray[0]->srcRect.w = BIG_NUMBER0_WIDTH;
	gameResources->bigNumberSpriteArray[0]->srcRect.h = BIG_NUMBER0_HEIGHT;
	gameResources->bigNumberSpriteArray[0]->destRect.x = 0;
	gameResources->bigNumberSpriteArray[0]->destRect.y = 0;
	gameResources->bigNumberSpriteArray[0]->destRect.w = BIG_NUMBER0_WIDTH;
	gameResources->bigNumberSpriteArray[0]->destRect.h = BIG_NUMBER0_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->bigNumberSpriteArray[0], 0, 0, (float)WINDOW_WIDTH / 15, (float)WINDOW_HEIGHT / 15);

	gameResources->bigNumberSpriteArray[1]->name = "big_number_1";
	gameResources->bigNumberSpriteArray[1]->xTranslation = 0;
	gameResources->bigNumberSpriteArray[1]->yTranslation = 0;
	gameResources->bigNumberSpriteArray[1]->angle = 0;
	gameResources->bigNumberSpriteArray[1]->gravity = 0;
	gameResources->bigNumberSpriteArray[1]->maxGravity = 0;
	gameResources->bigNumberSpriteArray[1]->lift = 0;
	gameResources->bigNumberSpriteArray[1]->maxLift = 0;
	gameResources->bigNumberSpriteArray[1]->srcRect.x = BIG_NUMBER1_XPOS;
	gameResources->bigNumberSpriteArray[1]->srcRect.y = BIG_NUMBER1_YPOS;
	gameResources->bigNumberSpriteArray[1]->srcRect.w = BIG_NUMBER1_WIDTH;
	gameResources->bigNumberSpriteArray[1]->srcRect.h = BIG_NUMBER1_HEIGHT;
	gameResources->bigNumberSpriteArray[1]->destRect.x = 0;
	gameResources->bigNumberSpriteArray[1]->destRect.y = 0;
	gameResources->bigNumberSpriteArray[1]->destRect.w = BIG_NUMBER1_WIDTH;
	gameResources->bigNumberSpriteArray[1]->destRect.h = BIG_NUMBER1_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->bigNumberSpriteArray[1], 0, 0, (float)WINDOW_WIDTH / 15, (float)WINDOW_HEIGHT / 15);
	gameResources->bigNumberSpriteArray[1]->destRect.x = gameResources->bigNumberSpriteArray[0]->destRect.x + gameResources->bigNumberSpriteArray[0]->destRect.w;

	gameResources->bigNumberSpriteArray[2]->name = "big_number_2";
	gameResources->bigNumberSpriteArray[2]->xTranslation = 0;
	gameResources->bigNumberSpriteArray[2]->yTranslation = 0;
	gameResources->bigNumberSpriteArray[2]->angle = 0;
	gameResources->bigNumberSpriteArray[2]->gravity = 0;
	gameResources->bigNumberSpriteArray[2]->maxGravity = 0;
	gameResources->bigNumberSpriteArray[2]->lift = 0;
	gameResources->bigNumberSpriteArray[2]->maxLift = 0;
	gameResources->bigNumberSpriteArray[2]->srcRect.x = BIG_NUMBER2_XPOS;
	gameResources->bigNumberSpriteArray[2]->srcRect.y = BIG_NUMBER2_YPOS;
	gameResources->bigNumberSpriteArray[2]->srcRect.w = BIG_NUMBER2_WIDTH;
	gameResources->bigNumberSpriteArray[2]->srcRect.h = BIG_NUMBER2_HEIGHT;
	gameResources->bigNumberSpriteArray[2]->destRect.x = 0;
	gameResources->bigNumberSpriteArray[2]->destRect.y = 0;
	gameResources->bigNumberSpriteArray[2]->destRect.w = BIG_NUMBER2_WIDTH;
	gameResources->bigNumberSpriteArray[2]->destRect.h = BIG_NUMBER2_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->bigNumberSpriteArray[2], 0, 0, (float)WINDOW_WIDTH / 15, (float)WINDOW_HEIGHT / 15);
	gameResources->bigNumberSpriteArray[2]->destRect.x = gameResources->bigNumberSpriteArray[1]->destRect.x + gameResources->bigNumberSpriteArray[1]->destRect.w;

	gameResources->bigNumberSpriteArray[3]->name = "big_number_3";
	gameResources->bigNumberSpriteArray[3]->xTranslation = 0;
	gameResources->bigNumberSpriteArray[3]->yTranslation = 0;
	gameResources->bigNumberSpriteArray[3]->angle = 0;
	gameResources->bigNumberSpriteArray[3]->gravity = 0;
	gameResources->bigNumberSpriteArray[3]->maxGravity = 0;
	gameResources->bigNumberSpriteArray[3]->lift = 0;
	gameResources->bigNumberSpriteArray[3]->maxLift = 0;
	gameResources->bigNumberSpriteArray[3]->srcRect.x = BIG_NUMBER3_XPOS;
	gameResources->bigNumberSpriteArray[3]->srcRect.y = BIG_NUMBER3_YPOS;
	gameResources->bigNumberSpriteArray[3]->srcRect.w = BIG_NUMBER3_WIDTH;
	gameResources->bigNumberSpriteArray[3]->srcRect.h = BIG_NUMBER3_HEIGHT;
	gameResources->bigNumberSpriteArray[3]->destRect.x = 0;
	gameResources->bigNumberSpriteArray[3]->destRect.y = 0;
	gameResources->bigNumberSpriteArray[3]->destRect.w = BIG_NUMBER3_WIDTH;
	gameResources->bigNumberSpriteArray[3]->destRect.h = BIG_NUMBER3_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->bigNumberSpriteArray[3], 0, 0, (float)WINDOW_WIDTH / 15, (float)WINDOW_HEIGHT / 15);
	gameResources->bigNumberSpriteArray[3]->destRect.x = gameResources->bigNumberSpriteArray[2]->destRect.x + gameResources->bigNumberSpriteArray[2]->destRect.w;

	gameResources->bigNumberSpriteArray[4]->name = "big_number_4";
	gameResources->bigNumberSpriteArray[4]->xTranslation = 0;
	gameResources->bigNumberSpriteArray[4]->yTranslation = 0;
	gameResources->bigNumberSpriteArray[4]->angle = 0;
	gameResources->bigNumberSpriteArray[4]->gravity = 0;
	gameResources->bigNumberSpriteArray[4]->maxGravity = 0;
	gameResources->bigNumberSpriteArray[4]->lift = 0;
	gameResources->bigNumberSpriteArray[4]->maxLift = 0;
	gameResources->bigNumberSpriteArray[4]->srcRect.x = BIG_NUMBER4_XPOS;
	gameResources->bigNumberSpriteArray[4]->srcRect.y = BIG_NUMBER4_YPOS;
	gameResources->bigNumberSpriteArray[4]->srcRect.w = BIG_NUMBER4_WIDTH;
	gameResources->bigNumberSpriteArray[4]->srcRect.h = BIG_NUMBER4_HEIGHT;
	gameResources->bigNumberSpriteArray[4]->destRect.x = 0;
	gameResources->bigNumberSpriteArray[4]->destRect.y = 0;
	gameResources->bigNumberSpriteArray[4]->destRect.w = BIG_NUMBER4_WIDTH;
	gameResources->bigNumberSpriteArray[4]->destRect.h = BIG_NUMBER4_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->bigNumberSpriteArray[4], 0, 0, (float)WINDOW_WIDTH / 15, (float)WINDOW_HEIGHT / 15);
	gameResources->bigNumberSpriteArray[4]->destRect.x = gameResources->bigNumberSpriteArray[3]->destRect.x + gameResources->bigNumberSpriteArray[3]->destRect.w;

	gameResources->bigNumberSpriteArray[5]->name = "big_number_5";
	gameResources->bigNumberSpriteArray[5]->xTranslation = 0;
	gameResources->bigNumberSpriteArray[5]->yTranslation = 0;
	gameResources->bigNumberSpriteArray[5]->angle = 0;
	gameResources->bigNumberSpriteArray[5]->gravity = 0;
	gameResources->bigNumberSpriteArray[5]->maxGravity = 0;
	gameResources->bigNumberSpriteArray[5]->lift = 0;
	gameResources->bigNumberSpriteArray[5]->maxLift = 0;
	gameResources->bigNumberSpriteArray[5]->srcRect.x = BIG_NUMBER5_XPOS;
	gameResources->bigNumberSpriteArray[5]->srcRect.y = BIG_NUMBER5_YPOS;
	gameResources->bigNumberSpriteArray[5]->srcRect.w = BIG_NUMBER5_WIDTH;
	gameResources->bigNumberSpriteArray[5]->srcRect.h = BIG_NUMBER5_HEIGHT;
	gameResources->bigNumberSpriteArray[5]->destRect.x = 0;
	gameResources->bigNumberSpriteArray[5]->destRect.y = 0;
	gameResources->bigNumberSpriteArray[5]->destRect.w = BIG_NUMBER5_WIDTH;
	gameResources->bigNumberSpriteArray[5]->destRect.h = BIG_NUMBER5_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->bigNumberSpriteArray[5], 0, 0, (float)WINDOW_WIDTH / 15, (float)WINDOW_HEIGHT / 15);
	gameResources->bigNumberSpriteArray[5]->destRect.x = gameResources->bigNumberSpriteArray[4]->destRect.x + gameResources->bigNumberSpriteArray[4]->destRect.w;

	gameResources->bigNumberSpriteArray[6]->name = "big_number_6";
	gameResources->bigNumberSpriteArray[6]->xTranslation = 0;
	gameResources->bigNumberSpriteArray[6]->yTranslation = 0;
	gameResources->bigNumberSpriteArray[6]->angle = 0;
	gameResources->bigNumberSpriteArray[6]->gravity = 0;
	gameResources->bigNumberSpriteArray[6]->maxGravity = 0;
	gameResources->bigNumberSpriteArray[6]->lift = 0;
	gameResources->bigNumberSpriteArray[6]->maxLift = 0;
	gameResources->bigNumberSpriteArray[6]->srcRect.x = BIG_NUMBER6_XPOS;
	gameResources->bigNumberSpriteArray[6]->srcRect.y = BIG_NUMBER6_YPOS;
	gameResources->bigNumberSpriteArray[6]->srcRect.w = BIG_NUMBER6_WIDTH;
	gameResources->bigNumberSpriteArray[6]->srcRect.h = BIG_NUMBER6_HEIGHT;
	gameResources->bigNumberSpriteArray[6]->destRect.x = 0;
	gameResources->bigNumberSpriteArray[6]->destRect.y = 0;
	gameResources->bigNumberSpriteArray[6]->destRect.w = BIG_NUMBER6_WIDTH;
	gameResources->bigNumberSpriteArray[6]->destRect.h = BIG_NUMBER6_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->bigNumberSpriteArray[6], 0, 0, (float)WINDOW_WIDTH / 15, (float)WINDOW_HEIGHT / 15);
	gameResources->bigNumberSpriteArray[6]->destRect.x = gameResources->bigNumberSpriteArray[5]->destRect.x + gameResources->bigNumberSpriteArray[5]->destRect.w;

	gameResources->bigNumberSpriteArray[7]->name = "big_number_7";
	gameResources->bigNumberSpriteArray[7]->xTranslation = 0;
	gameResources->bigNumberSpriteArray[7]->yTranslation = 0;
	gameResources->bigNumberSpriteArray[7]->angle = 0;
	gameResources->bigNumberSpriteArray[7]->gravity = 0;
	gameResources->bigNumberSpriteArray[7]->maxGravity = 0;
	gameResources->bigNumberSpriteArray[7]->lift = 0;
	gameResources->bigNumberSpriteArray[7]->maxLift = 0;
	gameResources->bigNumberSpriteArray[7]->srcRect.x = BIG_NUMBER7_XPOS;
	gameResources->bigNumberSpriteArray[7]->srcRect.y = BIG_NUMBER7_YPOS;
	gameResources->bigNumberSpriteArray[7]->srcRect.w = BIG_NUMBER7_WIDTH;
	gameResources->bigNumberSpriteArray[7]->srcRect.h = BIG_NUMBER7_HEIGHT;
	gameResources->bigNumberSpriteArray[7]->destRect.x = 0;
	gameResources->bigNumberSpriteArray[7]->destRect.y = 0;
	gameResources->bigNumberSpriteArray[7]->destRect.w = BIG_NUMBER7_WIDTH;
	gameResources->bigNumberSpriteArray[7]->destRect.h = BIG_NUMBER7_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->bigNumberSpriteArray[7], 0, 0, (float)WINDOW_WIDTH / 15, (float)WINDOW_HEIGHT / 15);
	gameResources->bigNumberSpriteArray[7]->destRect.x = gameResources->bigNumberSpriteArray[6]->destRect.x + gameResources->bigNumberSpriteArray[6]->destRect.w;

	gameResources->bigNumberSpriteArray[8]->name = "big_number_8";
	gameResources->bigNumberSpriteArray[8]->xTranslation = 0;
	gameResources->bigNumberSpriteArray[8]->yTranslation = 0;
	gameResources->bigNumberSpriteArray[8]->angle = 0;
	gameResources->bigNumberSpriteArray[8]->gravity = 0;
	gameResources->bigNumberSpriteArray[8]->maxGravity = 0;
	gameResources->bigNumberSpriteArray[8]->lift = 0;
	gameResources->bigNumberSpriteArray[8]->maxLift = 0;
	gameResources->bigNumberSpriteArray[8]->srcRect.x = BIG_NUMBER8_XPOS;
	gameResources->bigNumberSpriteArray[8]->srcRect.y = BIG_NUMBER8_YPOS;
	gameResources->bigNumberSpriteArray[8]->srcRect.w = BIG_NUMBER8_WIDTH;
	gameResources->bigNumberSpriteArray[8]->srcRect.h = BIG_NUMBER8_HEIGHT;
	gameResources->bigNumberSpriteArray[8]->destRect.x = 0;
	gameResources->bigNumberSpriteArray[8]->destRect.y = 0;
	gameResources->bigNumberSpriteArray[8]->destRect.w = BIG_NUMBER8_WIDTH;
	gameResources->bigNumberSpriteArray[8]->destRect.h = BIG_NUMBER8_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->bigNumberSpriteArray[8], 0, 0, (float)WINDOW_WIDTH / 15, (float)WINDOW_HEIGHT / 15);
	gameResources->bigNumberSpriteArray[8]->destRect.x = gameResources->bigNumberSpriteArray[7]->destRect.x + gameResources->bigNumberSpriteArray[7]->destRect.w;

	gameResources->bigNumberSpriteArray[9]->name = "big_number_9";
	gameResources->bigNumberSpriteArray[9]->xTranslation = 0;
	gameResources->bigNumberSpriteArray[9]->yTranslation = 0;
	gameResources->bigNumberSpriteArray[9]->angle = 0;
	gameResources->bigNumberSpriteArray[9]->gravity = 0;
	gameResources->bigNumberSpriteArray[9]->maxGravity = 0;
	gameResources->bigNumberSpriteArray[9]->lift = 0;
	gameResources->bigNumberSpriteArray[9]->maxLift = 0;
	gameResources->bigNumberSpriteArray[9]->srcRect.x = BIG_NUMBER9_XPOS;
	gameResources->bigNumberSpriteArray[9]->srcRect.y = BIG_NUMBER9_YPOS;
	gameResources->bigNumberSpriteArray[9]->srcRect.w = BIG_NUMBER9_WIDTH;
	gameResources->bigNumberSpriteArray[9]->srcRect.h = BIG_NUMBER9_HEIGHT;
	gameResources->bigNumberSpriteArray[9]->destRect.x = 0;
	gameResources->bigNumberSpriteArray[9]->destRect.y = 0;
	gameResources->bigNumberSpriteArray[9]->destRect.w = BIG_NUMBER9_WIDTH;
	gameResources->bigNumberSpriteArray[9]->destRect.h = BIG_NUMBER9_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->bigNumberSpriteArray[9], 0, 0, (float)WINDOW_WIDTH / 15, (float)WINDOW_HEIGHT / 15);
	gameResources->bigNumberSpriteArray[9]->destRect.x = gameResources->bigNumberSpriteArray[8]->destRect.x + gameResources->bigNumberSpriteArray[8]->destRect.w;

	return;
}

void UnloadGameResources(GameResources* gameResources)
{
	// Deallocate memory for background sprites 
	if (gameResources->backgroundSpriteArray)
	{
		free(gameResources->backgroundSpriteArray);
		gameResources->backgroundSpriteArray = NULL;
		if (DEBUG) printf("[DEBUG INFO] 'Background' sprites unloaded.\n");
	}

	// Deallocate memory for floor sprites
	if (gameResources->floorSpriteArray)
	{
		free(gameResources->floorSpriteArray);
		gameResources->floorSpriteArray = NULL;
		if (DEBUG) printf("[DEBUG INFO] 'Floor' sprites unloaded.\n");
	}

	// Deallocate memory for pillar pairs
	if (gameResources->pillarPairs)
	{
		for (int pillarPairIndex = 0; pillarPairIndex < gameResources->pillarPairCount; pillarPairIndex++)
		{
			DestroyPillarPair(&(gameResources->pillarPairs[pillarPairIndex]));
		}
		gameResources->pillarPairs = NULL;
		if (DEBUG) printf("[DEBUG INFO] Destroyed %d 'Pillar pairs'.\n", gameResources->pillarPairCount);
	}

	// Deallocate memory for flappy bird logo
	if (gameResources->flappyBirdLogo)
	{
		free(gameResources->flappyBirdLogo);
		gameResources->flappyBirdLogo = NULL;
		if (DEBUG) printf("[DEBUG INFO] 'Flappy bird logo' unloaded.\n");
	}

	// Deallocate memory for play button
	if (gameResources->playButton)
	{
		free(gameResources->playButton);
		gameResources->playButton = NULL;
		if (DEBUG) printf("[DEBUG INFO] 'Play' button unloaded.\n");
	}
	
	// Deallocate memory for leaderboards button
	if (gameResources->leaderboardButton)
	{
		free(gameResources->leaderboardButton);
		gameResources->leaderboardButton = NULL;
		if (DEBUG) printf("[DEBUG INFO] 'Leaderboard' button unloaded.\n");
	}

	// Deallocate memory for sprite containing 'Get Ready' text
	if (gameResources->getReady)
	{
		free(gameResources->getReady);
		gameResources->getReady = NULL;
		if (DEBUG) printf("[DEBUG INFO] 'Get Ready' sprite unloaded.\n");
	}

	// Deallocate memory for sprite containing 'Game Over' text
	if (gameResources->gameOver)
	{
		free(gameResources->gameOver);
		gameResources->gameOver = NULL;
		if (DEBUG) printf("[DEBUG INFO] 'Game Over' sprite unloaded.\n");
	}

	// Deallocate memory for 'Coming Soon' text
	if (gameResources->comingSoonText)
	{
		DestroyText(gameResources->comingSoonText);
		gameResources->comingSoonText = NULL;
	}

	// Deallocate memory for the tilemap
	if (gameResources->tileMap)
	{
		SDL_DestroyTexture(gameResources->tileMap);
		gameResources->tileMap = NULL;
		if (DEBUG) printf("[DEBUG INFO] Tilemap %s unloaded.\n", gameResources->tileMapPath);
	}
	return;
}

static Text* CreateText(const char* text, const int textSize, const SdlParameters* const sdlParameters, SDL_Color fontColor)
{
	TTF_Font* font;

	Text* newText = malloc(sizeof(Text));
	if (!newText)
	{
		printf("[ERROR] Memory allocation failed for Text!\n");
		return NULL;
	}

	// Set the default values
	newText->text = text;
	newText->destRect.x = 0;
	newText->destRect.y = 0;
	newText->destRect.w = 0;
	newText->destRect.h = 0;

	newText->textSize = textSize;
	font = TTF_OpenFont(sdlParameters->fontPath, newText->textSize);
	if (!font)
	{
		printf("[ERROR] TTF_OpenFont() failed!\n");
		return NULL;
	}
	
	// Create the surface
	newText->textSurface = TTF_RenderText_Blended(font, newText->text, fontColor);
	if (!newText->textSurface)
	{
		printf("[ERROR] TTF_RenderText_Blended() failed!\n");
		return NULL;
	}

	// Create the texture from the surface
	newText->textTexture = SDL_CreateTextureFromSurface(sdlParameters->renderer, newText->textSurface);
	if (!newText->textTexture)
	{
		printf("[ERROR] SDL_CreateTextureFromSurface() failed!\n");
		return NULL;
	}

	// Set the width and height of destRext of the text 
	// Note: Changing width and height to some other value will lead to improper font rendering
	newText->destRect.w = newText->textSurface->w;
	newText->destRect.h = newText->textSurface->h;
	
	TTF_CloseFont(font);
	if (DEBUG) printf("[DEBUG INFO] Text '%s' created.\n", newText->text);
	
	return newText;
}

static void DestroyText(Text* text)
{
	if (!text)
	{
		printf("Cannot destroy empty Text!\n");
		return;
	}

	if (DEBUG) printf("[DEBUG INFO] Destroying Text '%s'.\n", text->text);
	if (text->textSurface)
	{
		SDL_FreeSurface(text->textSurface);
		text->textSurface = NULL;
	}
	if (text->textTexture)
	{
		SDL_DestroyTexture(text->textTexture);
		text->textTexture = NULL;
	}
	if (text)
	{
		free(text);
	}
	if (DEBUG) printf("[DEBUG INFO] Text destroyed.\n");
	
	return;
}

static PillarPair* CreatePillarPairs(GameResources* gameResources, const SdlParameters* const sdlParameters, int pairCount)
{
	if (!gameResources)
	{
		printf("[ERROR] Cannot create pillar pairs from empty game resources!\n");
		return NULL;
	}

	// The pixel values on the tilemap from which the pillar will be rendered
	const int PILLAR_SRCRECT_XPOS = 56;
	const int PILLAR_SRCRECT_YPOS = 324;
	const int PILLAR_SRCRECT_WIDTH = 26;
	const int PILLAR_SRCRECT_HEIGHT = 158;
	const int PILLAR_SRCRECT_CAPITAL_HEIGHT = 16;

	const float PILLAR_SRCRECT_CAPITAL_HEIGHT_RATIO = (float)PILLAR_SRCRECT_CAPITAL_HEIGHT / PILLAR_SRCRECT_HEIGHT;	// Ratio of capital height to pillar height
	PillarPair* pillarPairs = NULL;

	// Allocate memory for the pillar pairs
	pillarPairs = (PillarPair*)calloc(sizeof(PillarPair), pairCount);
	if (!pillarPairs)
	{
		printf("[ERROR] Memory allocation failed for pillar pair!\n");
		return NULL;
	}
	
	// For each pillar pair, load the top pillar sprite and bottom pillar sprite
	for (int pillarPairsIndex = 0; pillarPairsIndex < pairCount; pillarPairsIndex++)
	{
		// Create the top pillar and set default values
		pillarPairs[pillarPairsIndex].topPillar = malloc(sizeof(Sprite));
		if (!pillarPairs[pillarPairsIndex].topPillar)
		{
			printf("[ERROR] Memory allocation failed for top pillar!\n");

			// Deallocate memory for previous pillar pairs if the memory allocation for current pillar pair fails
			for (int i = 0; i < pillarPairsIndex; i++)
			{
				if(pillarPairs[i].topPillar)
				{ 
					free(pillarPairs[i].topPillar);
				}
				pillarPairs = NULL;
			}
			return NULL;
		}
		pillarPairs[pillarPairsIndex].topPillar->name = "top_pillar";
		pillarPairs[pillarPairsIndex].topPillar->angle = 0;
		pillarPairs[pillarPairsIndex].topPillar->xTranslation = gameResources->parallaxGlobalXTranslation;
		pillarPairs[pillarPairsIndex].topPillar->yTranslation = 0;
		pillarPairs[pillarPairsIndex].topPillar->gravity = 0;
		pillarPairs[pillarPairsIndex].topPillar->maxGravity = 0;
		pillarPairs[pillarPairsIndex].topPillar->lift = 0;
		pillarPairs[pillarPairsIndex].topPillar->maxLift = 0;
		pillarPairs[pillarPairsIndex].topPillar->srcRect.x = PILLAR_SRCRECT_XPOS;
		pillarPairs[pillarPairsIndex].topPillar->srcRect.y = PILLAR_SRCRECT_YPOS;
		pillarPairs[pillarPairsIndex].topPillar->srcRect.w = PILLAR_SRCRECT_WIDTH;
		pillarPairs[pillarPairsIndex].topPillar->srcRect.h = PILLAR_SRCRECT_HEIGHT;
		
		// If this is the first pillar, place it at a distance gameResources->distanceBetweenPillars from the left side of the screen. If not, place it next to the previous pillar at a distance gameResources->distanceBetweenPillars
		pillarPairs[pillarPairsIndex].topPillar->destRect.x = 0;
		pillarPairs[pillarPairsIndex].topPillar->destRect.y = 0;
		pillarPairs[pillarPairsIndex].topPillar->destRect.w = PILLAR_SRCRECT_WIDTH;
		pillarPairs[pillarPairsIndex].topPillar->destRect.h = PILLAR_SRCRECT_HEIGHT;
		ScaleSpriteToFitOnArea(pillarPairs[pillarPairsIndex].topPillar, 0, 0, sdlParameters->dm.w, sdlParameters->dm.h - gameResources->floorSpriteArray[0].destRect.h);
		pillarPairs[pillarPairsIndex].topPillar->destRect.x = (pillarPairsIndex == 0) ? gameResources->distanceBetweenPillars : pillarPairs[pillarPairsIndex - 1].topPillar->destRect.x + pillarPairs[pillarPairsIndex - 1].topPillar->destRect.w + gameResources->distanceBetweenPillars;

		// Create the bottom pillar and set default values
		pillarPairs[pillarPairsIndex].bottomPillar = malloc(sizeof(Sprite));
		if (!pillarPairs[pillarPairsIndex].bottomPillar)
		{
			printf("[ERROR] Memory allocation failed for bottom pillar!\n");

			// Deallocate memory for previous pillar pairs if the memory allocation for current pillar pair fails
			for (int i = 0; i < pillarPairsIndex; i++)
			{
				// Check only whether the memory is allocated for the bottom pillar because the memory for 
				// the bottom pillar is allocated after allocating memory for the top pillar (Top pillar exists)
				if (pillarPairs[i].bottomPillar)
				{
					free(pillarPairs[i].topPillar);
					free(pillarPairs[i].bottomPillar);
				}
				pillarPairs = NULL;
			}
			return NULL;
		}
		pillarPairs[pillarPairsIndex].bottomPillar->name = "bottom_pillar";
		pillarPairs[pillarPairsIndex].bottomPillar->angle = 180;
		pillarPairs[pillarPairsIndex].bottomPillar->xTranslation = gameResources->parallaxGlobalXTranslation;
		pillarPairs[pillarPairsIndex].bottomPillar->yTranslation = 0;
		pillarPairs[pillarPairsIndex].bottomPillar->gravity = 0;
		pillarPairs[pillarPairsIndex].bottomPillar->maxGravity = 0;
		pillarPairs[pillarPairsIndex].bottomPillar->lift = 0;
		pillarPairs[pillarPairsIndex].bottomPillar->maxLift = 0;
		pillarPairs[pillarPairsIndex].bottomPillar->srcRect.x = PILLAR_SRCRECT_XPOS;
		pillarPairs[pillarPairsIndex].bottomPillar->srcRect.y = PILLAR_SRCRECT_YPOS;
		pillarPairs[pillarPairsIndex].bottomPillar->srcRect.w = PILLAR_SRCRECT_WIDTH;
		pillarPairs[pillarPairsIndex].bottomPillar->srcRect.h = PILLAR_SRCRECT_HEIGHT;

		// If this is the first pillar, place it at a distance gameResources->distanceBetweenPillars from the left side of the screen. If not, place it next to the previous pillar at a distance gameResources->distanceBetweenPillars
		pillarPairs[pillarPairsIndex].bottomPillar->destRect.x = 0;
		pillarPairs[pillarPairsIndex].bottomPillar->destRect.y = 0;
		pillarPairs[pillarPairsIndex].bottomPillar->destRect.w = PILLAR_SRCRECT_WIDTH;
		pillarPairs[pillarPairsIndex].bottomPillar->destRect.h = PILLAR_SRCRECT_HEIGHT;
		ScaleSpriteToFitOnArea(pillarPairs[pillarPairsIndex].bottomPillar, 0, 0, sdlParameters->dm.w, sdlParameters->dm.h - gameResources->floorSpriteArray[0].destRect.h);
		pillarPairs[pillarPairsIndex].bottomPillar->destRect.x = (pillarPairsIndex == 0) ? gameResources->distanceBetweenPillars : pillarPairs[pillarPairsIndex - 1].bottomPillar->destRect.x + pillarPairs[pillarPairsIndex - 1].bottomPillar->destRect.w + gameResources->distanceBetweenPillars;

		// Set random pillar heights
		SetRandomPillarHeight(&(pillarPairs[pillarPairsIndex]), gameResources, sdlParameters);
	}
	
	// Get the capital height after resizing the pillars to fit the height of the window
	gameResources->pillarCapitalHeight = (double) pillarPairs->topPillar[0].destRect.h * PILLAR_SRCRECT_CAPITAL_HEIGHT_RATIO;

	if (DEBUG) printf("[DEBUG INFO] Created 'Pillar pairs'. Count = %d\n", pairCount);
	if (DEBUG) printf("[DEBUG INFO] Capital height of pillar = %d\n", gameResources->pillarCapitalHeight);
	return pillarPairs;
}

static void DestroyPillarPair(PillarPair* pillarPair)
{
	if (pillarPair->topPillar)
	{
		free(pillarPair->topPillar);
	}
	if (pillarPair->bottomPillar)
	{
		free(pillarPair->bottomPillar);
	}
	return;
}

static void SetRandomPillarHeight(PillarPair* pillarPair, GameResources* gameResources, const SdlParameters* const sdlParameters)
{
	if (!pillarPair)
	{
		printf("[ERROR] Cannot set random pillar height to empty pillar pair!\n");
		return;
	}
	else if (!gameResources)
	{
		printf("[ERROR] Cannot set random pillar height with empty game resources!\n");
		return;
	}
	else if (!sdlParameters)
	{
		printf("[ERROR] Cannot set random pillar height with empty SDL parameters!\n");
		return;
	}

	int min, max, randomHeight;

	// Calculate random pillar height for top pillar and set the yPosition of the top pillar
	min = gameResources->pillarCapitalHeight;
	max = sdlParameters->dm.h - (gameResources->pillarCapitalHeight + gameResources->pillarPairMinimumSpacing + gameResources->floorSpriteArray[0].destRect.h);
	randomHeight = (rand() % (max + 1 - min)) + min;
	pillarPair->topPillar->destRect.y = (-1 * pillarPair->topPillar->destRect.h) + randomHeight;
	
	// Prevent the pillars from going completely out of the window
	if ((pillarPair->topPillar->destRect.y + pillarPair->topPillar->destRect.h) < min)
	{
		pillarPair->topPillar->destRect.y = (-1 * pillarPair->topPillar->destRect.h) + min;
	}
	else if ((pillarPair->topPillar->destRect.y + pillarPair->topPillar->destRect.h) > max)
	{
		pillarPair->topPillar->destRect.y = (-1 * pillarPair->topPillar->destRect.h) + max;
	}

	// Set the yPosition for the bottom pillar according to the yPosition of the top pillar
	pillarPair->bottomPillar->destRect.y = pillarPair->topPillar->destRect.y + pillarPair->topPillar->destRect.h + gameResources->pillarPairMinimumSpacing;

	return;
}

void ParallaxEffect(Sprite* sprites, int spriteCount, int* const leftEndIndex, int* const rightEndIndex)
{
	int left = *leftEndIndex;
	int right = *rightEndIndex;

	if (!sprites)
	{
		printf("[ERROR] Cannot create parallax effect with empty sprites!\n");
		return;
	}
	else if (!leftEndIndex)
	{
		printf("[ERROR] Cannot create parallax effect without left index value!\n");
		return;
	}
	else if (!rightEndIndex)
	{
		printf("[ERROR] Cannot create parallax effect without right index value!\n");
		return;
	}
	else if (spriteCount <= 0)
	{
		printf("[ERROR] Cannot create parallax effect with %d sprites!\n", spriteCount);
		return;
	}

	// Move the backgrounds
	for (int spriteIndex = 0; spriteIndex < spriteCount; spriteIndex++)
	{
		// Move the background towards left
		sprites[spriteIndex].destRect.x += sprites[spriteIndex].xTranslation;
	}

	// If the background at the left end goes out of screen
	if (sprites[left].destRect.x + sprites[left].destRect.w < 0)
	{
		sprites[left].destRect.x = sprites[right].destRect.x + sprites[right].destRect.w;
	}

	// Wraparound - Indices cannot go beyond (gameResources->backgroundSpriteCount - 1)
	*leftEndIndex = (left + 1) % spriteCount;
	*rightEndIndex = (right + 1) % spriteCount;

	return;
}

void CenterSpriteOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters)
{
	if (!sprite)
	{
		printf("[ERROR] Cannot center empty sprite!\n");
		return;
	}
	sprite->destRect.x = ((float)sdlParameters->dm.w / 2) - ((float)sprite->destRect.w / 2);
	sprite->destRect.y = ((float)sdlParameters->dm.h / 2) - ((float)sprite->destRect.h / 2);

	if (DEBUG) printf("[DEBUG INFO] Centering sprite '%s' on screen. destX = %d, destY = %d, destW = %d, destH = %d\n", sprite->name, sprite->destRect.x, sprite->destRect.y, sprite->destRect.w, sprite->destRect.h);
	return;
}

void CenterSpriteHorizontallyOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters)
{
	if (!sprite)
	{
		printf("[ERROR] Cannot center empty sprite!\n");
		return;
	}
	sprite->destRect.x = ((float)sdlParameters->dm.w / 2) - ((float)sprite->destRect.w / 2);

	if (DEBUG) printf("[DEBUG INFO] Centering sprite '%s' vertically on screen. destX = %d, destY = %d, destW = %d, destH = %d\n", sprite->name, sprite->destRect.x, sprite->destRect.y, sprite->destRect.w, sprite->destRect.h);
	return;
}

void CenterSpriteVerticallyOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters)
{
	if (!sprite)
	{
		printf("[ERROR] Cannot center empty sprite!\n");
		return;
	}
	sprite->destRect.y = ((float)sdlParameters->dm.h / 2) - ((float)sprite->destRect.h / 2);

	if (DEBUG) printf("[DEBUG INFO] Centering sprite '%s' horizontally on screen. destX = %d, destY = %d, destW = %d, destH = %d\n", sprite->name, sprite->destRect.x, sprite->destRect.y, sprite->destRect.w, sprite->destRect.h);
	return;
}

// Set the destRect's height and width to the same as that of the srcRect before calling this function!
void ScaleSpriteToFitOnArea(Sprite* sprite, int destX, int destY, int destWidth, int destHeight)
{
	if (!sprite)
	{
		printf("[ERROR] Cannot scale empty sprite!\n");
		return;
	}

	float widthToHeightRatio = (float)sprite->destRect.w / sprite->destRect.h;
	float heightToWidthRatio = (float)sprite->destRect.h / sprite->destRect.w;
	
	sprite->destRect.x = destX;
	sprite->destRect.y = destY;
	
	if (sprite->destRect.w < sprite->destRect.h)
	{
		sprite->destRect.h = destHeight;
		sprite->destRect.w = widthToHeightRatio * sprite->destRect.h;
	}
	else
	{
		sprite->destRect.w = destWidth;
		sprite->destRect.h = heightToWidthRatio * sprite->destRect.w;
	}

	if (DEBUG) printf("[DEBUG INFO] Scaling sprite '%s' to fit. destX = %d, destY = %d, destW = %d, destH = %d\n", sprite->name, sprite->destRect.x, sprite->destRect.y, sprite->destRect.w, sprite->destRect.h);
	return;
}

// Used to scale sprite in-place (By automatically calculating the offsets)
void ScaleSpriteInPlaceByFactor(Sprite* sprite, float factor)
{
	int centerX, centerY;
	float scaledDestWidth, scaledDestHeight;

	if (factor < 0)
	{
		printf("[ERROR] Scaling factor %f cannot be lesser than 0\n", factor);
		return;
	}

	if (factor == 0 || factor == 1)
	{
		if(DEBUG) printf("[DEBUG INFO] Scaling sprite '%s' by factor %f!\n", sprite->name, factor);
		return;
	}

	centerX = sprite->destRect.x + ((float)sprite->destRect.w / 2);
	centerY = sprite->destRect.y + ((float)sprite->destRect.h / 2);
	scaledDestWidth = sprite->destRect.w * factor;
	scaledDestHeight = sprite->destRect.h * factor;

	sprite->destRect.x = centerX - ((float)scaledDestWidth / 2);
	sprite->destRect.y = centerY - ((float)scaledDestHeight / 2);

	sprite->destRect.w = scaledDestWidth;
	sprite->destRect.h = scaledDestHeight;

	if (DEBUG) printf("[DEBUG INFO] Scaling sprite '%s' by factor %f.\n", sprite->name, factor);
	return;
}
