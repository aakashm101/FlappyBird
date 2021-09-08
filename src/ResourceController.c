#include <stdio.h>
#include <SDL2/SDL_image.h>
#include "global.h"
#include "ResourceController.h"

int LoadGameResources(GameResources* gameResources, const SdlParameters* const sdlParameters)
{
	// Fetch the window dimension values
	const int WINDOW_WIDTH = sdlParameters->dm.w;
	const int WINDOW_HEIGHT = sdlParameters->dm.h;

	const SDL_Color fontColor = { 0xFF, 0xFF, 0xFF, 0xFF };

	// Background X,Y coordinates, width and height (Obtained from tilemap)
	const int BACKGROUND_XPOS = 0;
	const int BACKGROUND_YPOS = 0;
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

	// Scale the background image to the window size without affecting dimensions of floor image
	const double BACKGROUND_WIDTH_HEIGHT_RATIO = (double) BACKGROUND_WIDTH / BACKGROUND_HEIGHT;
	const int SCALED_BACKGROUND_HEIGHT = WINDOW_HEIGHT;
	const int SCALED_BACKGROUND_WIDTH = ceil(SCALED_BACKGROUND_HEIGHT * BACKGROUND_WIDTH_HEIGHT_RATIO);

	// Scale the floor image to the window height / 5 without affecting dimensions of the floor image
	const double FLOOR_WIDTH_HEIGHT_RATIO = (double)FLOOR_WIDTH / FLOOR_HEIGHT;
	const int SCALED_FLOOR_HEIGHT = ceil(WINDOW_HEIGHT / 5);
	const int SCALED_FLOOR_WIDTH = ceil(SCALED_FLOOR_HEIGHT * FLOOR_WIDTH_HEIGHT_RATIO);

	int backgroundImageCount;
	int floorImageCount;

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
	backgroundImageCount = ceil((double)WINDOW_WIDTH / SCALED_BACKGROUND_WIDTH);
	backgroundImageCount = backgroundImageCount + 1;
	gameResources->backgroundSpriteCount = backgroundImageCount;
	gameResources->backgroundLeftEndIndex = 0;
	gameResources->backgroundRightEndIndex = backgroundImageCount - 1;

	// Set the srcRect and destRect and default values for background images from the tilemap
	gameResources->backgroundSpriteArray = (Sprite*)calloc(backgroundImageCount, sizeof(Sprite));
	if (!gameResources->backgroundSpriteArray)
	{
		printf("[ERROR] Failed to allocate memory for backround sprite!\n");
		return -1;
	}
	for (int backgroundSpriteIndex = 0; backgroundSpriteIndex < backgroundImageCount; backgroundSpriteIndex++)
	{
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].name = "background";
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].angle = 0;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].xTranslation = -1;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].yTranslation = 0;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].srcRect.x = BACKGROUND_XPOS;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].srcRect.y = BACKGROUND_YPOS;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].srcRect.w = BACKGROUND_WIDTH;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].srcRect.h = BACKGROUND_HEIGHT;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.x = (backgroundSpriteIndex == 0) ? 0 : gameResources->backgroundSpriteArray[backgroundSpriteIndex - 1].destRect.x + gameResources->backgroundSpriteArray[backgroundSpriteIndex - 1].destRect.w;	// Place the backgrounds size by side
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.y = 0;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.w = BACKGROUND_WIDTH;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.h = BACKGROUND_HEIGHT;
		ScaleSpriteToFitOnArea(
			&(gameResources->backgroundSpriteArray[backgroundSpriteIndex]),
			gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.x,
			gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.y,
			SCALED_BACKGROUND_WIDTH,
			SCALED_BACKGROUND_HEIGHT);
	}
	if (DEBUG) printf("[DEBUG INFO] '%s' sprites ready.\n", gameResources->backgroundSpriteArray[0].name);

	// Used to store how many floor images will fit inside the window (Using multiple scrolling floor images to create parallax effect)
	// Use one more floor image (It can be partially outside the current window) to create the parallax effect
	floorImageCount = ceil((double)WINDOW_WIDTH / SCALED_FLOOR_WIDTH);
	floorImageCount = floorImageCount + 1;
	gameResources->floorSpriteCount = floorImageCount;
	gameResources->floorLeftEndIndex = 0;
	gameResources->floorRightEndIndex = floorImageCount - 1;

	// Set the srcRect and destRect and default values for floor images from the tilemap
	gameResources->floorSpriteArray = (Sprite*)calloc(floorImageCount, sizeof(Sprite));
	if (!gameResources->floorSpriteArray)
	{
		printf("[ERROR] Failed to allocate memory for floor sprite!\n");
		return -1;
	}
	for (int floorSpriteIndex = 0; floorSpriteIndex < floorImageCount; floorSpriteIndex++)
	{
		gameResources->floorSpriteArray[floorSpriteIndex].name = "floor";
		gameResources->floorSpriteArray[floorSpriteIndex].angle = 0;
		gameResources->floorSpriteArray[floorSpriteIndex].xTranslation = -1;
		gameResources->floorSpriteArray[floorSpriteIndex].yTranslation = 0;
		gameResources->floorSpriteArray[floorSpriteIndex].srcRect.x = FLOOR_XPOS;
		gameResources->floorSpriteArray[floorSpriteIndex].srcRect.y = FLOOR_YPOS;
		gameResources->floorSpriteArray[floorSpriteIndex].srcRect.w = FLOOR_WIDTH;
		gameResources->floorSpriteArray[floorSpriteIndex].srcRect.h = FLOOR_HEIGHT;
		gameResources->floorSpriteArray[floorSpriteIndex].destRect.x = (floorSpriteIndex == 0) ? 0 : gameResources->floorSpriteArray[floorSpriteIndex - 1].destRect.x + gameResources->floorSpriteArray[floorSpriteIndex - 1].destRect.w;	// Place the floors size by side
		gameResources->floorSpriteArray[floorSpriteIndex].destRect.y = WINDOW_HEIGHT - ((double)WINDOW_HEIGHT / 5);
		gameResources->floorSpriteArray[floorSpriteIndex].destRect.w = SCALED_FLOOR_WIDTH;
		gameResources->floorSpriteArray[floorSpriteIndex].destRect.h = SCALED_FLOOR_HEIGHT;
		ScaleSpriteToFitOnArea(
			&(gameResources->floorSpriteArray[floorSpriteIndex]),
			gameResources->floorSpriteArray[floorSpriteIndex].destRect.x,
			gameResources->floorSpriteArray[floorSpriteIndex].destRect.y,
			SCALED_FLOOR_WIDTH,
			SCALED_FLOOR_HEIGHT);
	}
	if (DEBUG) printf("[DEBUG INFO] '%s' sprites ready.\n", gameResources->floorSpriteArray[0].name);

	// Set the srcRect and destRect and default values for flappy bird logo from the tilemap
	// Fit the flappy bird logo to the top 1/3 of the area of the screen
	gameResources->flappyBirdLogo = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->flappyBirdLogo)
	{
		printf("[ERROR] Memory allocation failed for flappy bird logo!\n");
		return -1;
	}
	gameResources->flappyBirdLogo->name = "flappy_bird_logo";
	gameResources->flappyBirdLogo->angle = 0;
	gameResources->flappyBirdLogo->xTranslation = 0;
	gameResources->flappyBirdLogo->yTranslation = 0;
	gameResources->flappyBirdLogo->srcRect.x = FLAPPY_BIRD_LOGO_XPOS;
	gameResources->flappyBirdLogo->srcRect.y = FLAPPY_BIRD_LOGO_YPOS;
	gameResources->flappyBirdLogo->srcRect.w = FLAPPY_BIRD_LOGO_WIDTH;
	gameResources->flappyBirdLogo->srcRect.h = FLAPPY_BIRD_LOGO_HEIGHT;
	gameResources->flappyBirdLogo->destRect.x = 0;
	gameResources->flappyBirdLogo->destRect.y = 0;
	gameResources->flappyBirdLogo->destRect.w = FLAPPY_BIRD_LOGO_WIDTH;
	gameResources->flappyBirdLogo->destRect.h = FLAPPY_BIRD_LOGO_HEIGHT;
	ScaleSpriteToFitOnArea(gameResources->flappyBirdLogo, 0, 0, WINDOW_WIDTH, (double)WINDOW_HEIGHT / 3);
	CenterSpriteHorizontallyOnScreen(gameResources->flappyBirdLogo, sdlParameters);
	ScaleSpriteInPlaceByFactor(gameResources->flappyBirdLogo, 0.8);
	if (DEBUG) printf("[DEBUG INFO] '%s' sprite ready.\n", gameResources->flappyBirdLogo->name);

	// Set the srcRect and destRect and default values for play button from the tilemap
	gameResources->playButton = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->playButton)
	{
		printf("[ERROR] Memory allocation failed for play button!\n");
		return -1;
	}
	gameResources->playButton->name = "play_button";
	gameResources->playButton->angle = 0;
	gameResources->playButton->xTranslation = 0;
	gameResources->playButton->yTranslation = 0;
	gameResources->playButton->srcRect.x = PLAY_BUTTON_XPOS;
	gameResources->playButton->srcRect.y = PLAY_BUTTON_YPOS;
	gameResources->playButton->srcRect.w = PLAY_BUTTON_WIDTH;
	gameResources->playButton->srcRect.h = PLAY_BUTTON_HEIGHT;
	gameResources->playButton->destRect.x = 0;
	gameResources->playButton->destRect.y = ((double)WINDOW_HEIGHT / 2) + PLAY_BUTTON_HEIGHT;
	gameResources->playButton->destRect.w = (double)WINDOW_WIDTH / 2;
	gameResources->playButton->destRect.h = (double)WINDOW_HEIGHT / 2;
	ScaleSpriteInPlaceByFactor(gameResources->playButton, 0.5);
	if (DEBUG) printf("[DEBUG INFO] '%s' sprite ready.\n", gameResources->playButton->name);

	// Set the srcRect and destRect and default values for leaderboard button from the tilemap
	gameResources->leaderboardButton = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->leaderboardButton)
	{
		printf("[ERROR] Memory allocation failed for leaderboard button!\n");
		return -1;
	}
	gameResources->leaderboardButton->name = "leaderboard_button";
	gameResources->leaderboardButton->angle = 0;
	gameResources->leaderboardButton->xTranslation = 0;
	gameResources->leaderboardButton->yTranslation = 0;
	gameResources->leaderboardButton->srcRect.x = LEADERBOARD_BUTTON_XPOS;
	gameResources->leaderboardButton->srcRect.y = LEADERBOARD_BUTTON_YPOS;
	gameResources->leaderboardButton->srcRect.w = LEADERBOARD_BUTTON_WIDTH;
	gameResources->leaderboardButton->srcRect.h = LEADERBOARD_BUTTON_HEIGHT;
	gameResources->leaderboardButton->destRect.x = (double)WINDOW_WIDTH / 2;
	gameResources->leaderboardButton->destRect.y = ((double)WINDOW_HEIGHT / 2) + LEADERBOARD_BUTTON_HEIGHT;
	gameResources->leaderboardButton->destRect.w = (double)WINDOW_WIDTH / 2;
	gameResources->leaderboardButton->destRect.h = (double)WINDOW_HEIGHT / 2;
	ScaleSpriteInPlaceByFactor(gameResources->leaderboardButton, 0.5);
	if (DEBUG) printf("[DEBUG INFO] '%s' sprite ready.\n", gameResources->leaderboardButton->name);

	// Set the srcRect and destRect and default values for sprite containing 'Get Ready' text from the tilemap
	// Place it at WINDOW HEIGHT/6 distance from the top and scale the texture to fit
	gameResources->getReady = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->getReady)
	{
		printf("[ERROR] Memory allocation failed for sprite containing 'Get Ready' text!\n");
		return -1;
	}
	gameResources->getReady->name = "get_ready";
	gameResources->getReady->angle = 0;
	gameResources->getReady->xTranslation = 0;
	gameResources->getReady->yTranslation = 0;
	gameResources->getReady->srcRect.x = 294;
	gameResources->getReady->srcRect.y = 58;
	gameResources->getReady->srcRect.w = 94;
	gameResources->getReady->srcRect.h = 28;
	gameResources->getReady->destRect.x = 0;
	gameResources->getReady->destRect.y = WINDOW_HEIGHT / 6;
	gameResources->getReady->destRect.w = gameResources->getReady->srcRect.w;
	gameResources->getReady->destRect.h = gameResources->getReady->srcRect.h;
	ScaleSpriteToFitOnArea(gameResources->getReady, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT / 6);
	CenterSpriteHorizontallyOnScreen(gameResources->getReady, sdlParameters);
	ScaleSpriteInPlaceByFactor(gameResources->getReady, 0.5);

	// Set the srcRect and destRect and default values for sprite containing 'Game Over' text from the tilemap
	// Place it at WINDOW HEIGHT/6 distance from the top and scale the texture to fit
	gameResources->gameOver = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->gameOver)
	{
		printf("[ERROR] Memory allocation failed for sprite containing 'Game Over' text!\n");
		return -1;
	}
	gameResources->gameOver->name = "game_over";
	gameResources->gameOver->angle = 0;
	gameResources->gameOver->xTranslation = 0;
	gameResources->gameOver->yTranslation = 0;
	gameResources->gameOver->srcRect.x = 394;
	gameResources->gameOver->srcRect.y = 58;
	gameResources->gameOver->srcRect.w = 98;
	gameResources->gameOver->srcRect.h = 28;
	gameResources->gameOver->destRect.x = 0;
	gameResources->gameOver->destRect.y = WINDOW_HEIGHT / 6;
	gameResources->gameOver->destRect.w = gameResources->gameOver->srcRect.w;
	gameResources->gameOver->destRect.h = gameResources->gameOver->srcRect.h;
	ScaleSpriteToFitOnArea(gameResources->gameOver, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT / 6);
	CenterSpriteHorizontallyOnScreen(gameResources->gameOver, sdlParameters);
	ScaleSpriteInPlaceByFactor(gameResources->gameOver, 0.5);

	// Allocate memory for 'Coming soon' Text
	gameResources->comingSoonText = CreateText("Coming Soon", 40, sdlParameters, fontColor);
	if (!gameResources->comingSoonText)
	{
		printf("[ERROR] Text creation failed!\n");
		return -1;
	}

	gameResources->pillarPairCount = 8;
	gameResources->pillarPairsLeftEndIndex = 0;
	gameResources->pillarPairsRightEndIndex = gameResources->pillarPairCount - 1;
	gameResources->distanceBetweenPillars = 200;
	gameResources->pillarPairs = CreatePillarPair(gameResources, sdlParameters, gameResources->pillarPairCount);
	if (!gameResources->pillarPairs)
	{
		return -1;
	}

	return 0;
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

	// Deallocate memory for the tilemap
	if (gameResources->tileMap)
	{
		SDL_DestroyTexture(gameResources->tileMap);
		gameResources->tileMap = NULL;
		if (DEBUG) printf("[DEBUG INFO] Tilemap %s unloaded.\n", gameResources->tileMapPath);
	}

	// Deallocate memory for 'Coming Soon' text
	if (gameResources->comingSoonText)
	{
		DestroyText(gameResources->comingSoonText);
		gameResources->comingSoonText = NULL;
	}

	return;
}

Text* CreateText(const char* text, const int textSize, SdlParameters* const sdlParameters, SDL_Color fontColor)
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

void DestroyText(Text* text)
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

PillarPair* CreatePillarPair(GameResources* gameResources, SdlParameters* sdlParameters, int pairCount)
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
	const int PILLAR_SRCRECT_HEIGHT = 162;

	// Downscaling the pillar width if the pillar is too wide
	const double PILLAR_DESTRECT_WIDTH_DOWNSCALING_FACTOR = 1.3;

	// Fetch the screen dimensions once instead of fetching it multiple times during the calculation
	int windowWidth = sdlParameters->dm.w;
	int windowHeight = sdlParameters->dm.h;

	// Declare the variable
	PillarPair* pillarPair = NULL;

	// Allocate memory for the pillar pairs
	pillarPair = (PillarPair*)calloc(sizeof(PillarPair), pairCount);
	if (!pillarPair)
	{
		printf("[ERROR] Memory allocation failed for pillar pair!\n");
		return NULL;
	}
	
	// For each pillar pair, load the top pillar sprite and bottom pillar sprite
	for (int pillarPairIndex = 0; pillarPairIndex < pairCount; pillarPairIndex++)
	{
		// Create the top pillar and set default values
		pillarPair[pillarPairIndex].topPillar = malloc(sizeof(Sprite));
		if (!pillarPair[pillarPairIndex].topPillar)
		{
			printf("[ERROR] Memory allocation failed for top pillar!\n");		// Need to improvise memory deallocation later
			return NULL;
		}
		pillarPair[pillarPairIndex].topPillar->name = "top_pillar";
		pillarPair[pillarPairIndex].topPillar->angle = 0;
		pillarPair[pillarPairIndex].topPillar->xTranslation = -1;
		pillarPair[pillarPairIndex].topPillar->yTranslation = 0;
		pillarPair[pillarPairIndex].topPillar->srcRect.x = PILLAR_SRCRECT_XPOS;
		pillarPair[pillarPairIndex].topPillar->srcRect.y = PILLAR_SRCRECT_YPOS;
		pillarPair[pillarPairIndex].topPillar->srcRect.w = PILLAR_SRCRECT_WIDTH;
		pillarPair[pillarPairIndex].topPillar->srcRect.h = PILLAR_SRCRECT_HEIGHT;
		
		// If this is the first pillar, place it at a distance gameResources->distanceBetweenPillars from the left side of the screen. If not, place it next to the previous pillar at a distance gameResources->distanceBetweenPillars
		pillarPair[pillarPairIndex].topPillar->destRect.x = 0;
		pillarPair[pillarPairIndex].topPillar->destRect.y = 0;
		pillarPair[pillarPairIndex].topPillar->destRect.w = PILLAR_SRCRECT_WIDTH;
		pillarPair[pillarPairIndex].topPillar->destRect.h = PILLAR_SRCRECT_HEIGHT;
		ScaleSpriteToFitOnArea(pillarPair[pillarPairIndex].topPillar, 0, 0, windowWidth, windowHeight - gameResources->floorSpriteArray[0].destRect.h);
		pillarPair[pillarPairIndex].topPillar->destRect.x = (pillarPairIndex == 0) ? gameResources->distanceBetweenPillars : pillarPair[pillarPairIndex - 1].topPillar->destRect.x + pillarPair[pillarPairIndex - 1].topPillar->destRect.w + gameResources->distanceBetweenPillars;
		pillarPair[pillarPairIndex].topPillar->destRect.w /= PILLAR_DESTRECT_WIDTH_DOWNSCALING_FACTOR;	// Reduce the width of the pillar
		pillarPair[pillarPairIndex].topPillar->destRect.y -= 500;	// Testing

		// Create the bottom pillar and set default values
		pillarPair[pillarPairIndex].bottomPillar = malloc(sizeof(Sprite));
		if (!pillarPair[pillarPairIndex].bottomPillar)			// Need to improvise memory deallocation later
		{
			printf("[ERROR] Memory allocation failed for bottom pillar!\n");
			return NULL;
		}
		pillarPair[pillarPairIndex].bottomPillar->name = "bottom_pillar";
		pillarPair[pillarPairIndex].bottomPillar->angle = 180;
		pillarPair[pillarPairIndex].bottomPillar->xTranslation = -1;
		pillarPair[pillarPairIndex].bottomPillar->yTranslation = 0;
		pillarPair[pillarPairIndex].bottomPillar->srcRect.x = PILLAR_SRCRECT_XPOS;
		pillarPair[pillarPairIndex].bottomPillar->srcRect.y = PILLAR_SRCRECT_YPOS;
		pillarPair[pillarPairIndex].bottomPillar->srcRect.w = PILLAR_SRCRECT_WIDTH;
		pillarPair[pillarPairIndex].bottomPillar->srcRect.h = PILLAR_SRCRECT_HEIGHT;

		// If this is the first pillar, place it at a distance gameResources->distanceBetweenPillars from the left side of the screen. If not, place it next to the previous pillar at a distance gameResources->distanceBetweenPillars
		pillarPair[pillarPairIndex].bottomPillar->destRect.x = 0;
		pillarPair[pillarPairIndex].bottomPillar->destRect.y = 0;
		pillarPair[pillarPairIndex].bottomPillar->destRect.w = PILLAR_SRCRECT_WIDTH;
		pillarPair[pillarPairIndex].bottomPillar->destRect.h = PILLAR_SRCRECT_HEIGHT;
		ScaleSpriteToFitOnArea(pillarPair[pillarPairIndex].bottomPillar, 0, 0, windowWidth, windowHeight - gameResources->floorSpriteArray[0].destRect.h);
		pillarPair[pillarPairIndex].bottomPillar->destRect.x = (pillarPairIndex == 0) ? gameResources->distanceBetweenPillars : pillarPair[pillarPairIndex - 1].bottomPillar->destRect.x + pillarPair[pillarPairIndex - 1].bottomPillar->destRect.w + gameResources->distanceBetweenPillars;
		pillarPair[pillarPairIndex].bottomPillar->destRect.w /= PILLAR_DESTRECT_WIDTH_DOWNSCALING_FACTOR;	// Reduce the width of the pillar
		pillarPair[pillarPairIndex].bottomPillar->destRect.y += 500;	// Testing
	}
	
	if (DEBUG) printf("[DEBUG INFO] Created pillar pair\n");
	return pillarPair;
}

void ParallaxEffect(Sprite* sprites, int spriteCount, int* const leftEndIndex, int* const rightEndIndex)
{
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
	if (sprites[*leftEndIndex].destRect.x + sprites[*leftEndIndex].destRect.w < 0)
	{
		sprites[*leftEndIndex].destRect.x = sprites[*rightEndIndex].destRect.x + sprites[*rightEndIndex].destRect.w;
	}

	// Wraparound - Indices cannot go beyond (gameResources->backgroundSpriteCount - 1)
	*leftEndIndex = (*leftEndIndex + 1) % spriteCount;
	*rightEndIndex = (*rightEndIndex + 1) % spriteCount;

	return;
}

void CenterSpriteOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters)
{
	if (!sprite)
	{
		printf("[ERROR] Cannot center empty sprite!\n");
		return;
	}
	sprite->destRect.x = ((double)sdlParameters->dm.w / 2) - ((double)sprite->destRect.w / 2);
	sprite->destRect.y = ((double)sdlParameters->dm.h / 2) - ((double)sprite->destRect.h / 2);

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
	sprite->destRect.x = ((double)sdlParameters->dm.w / 2) - ((double)sprite->destRect.w / 2);

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
	sprite->destRect.y = ((double)sdlParameters->dm.h / 2) - ((double)sprite->destRect.h / 2);

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

	double widthToHeightRatio = (double)sprite->destRect.w / sprite->destRect.h;
	double heightToWidthRatio = (double)sprite->destRect.h / sprite->destRect.w;
	
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
