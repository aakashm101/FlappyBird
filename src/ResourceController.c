#include <stdio.h>
#include <SDL2/SDL_image.h>
#include "global.h"
#include "ResourceController.h"

int LoadGameResources(GameResources* gameResources, const SdlParameters* const sdlParameters)
{
	const int WINDOW_AREA = sdlParameters->dm.w * sdlParameters->dm.h;

	// Background X,Y coordinates, width and height (Obtained from tilemap)
	const int BACKGROUND_XPOS = 0;
	const int BACKGROUND_YPOS = 0;
	const int BACKGROUND_WIDTH = 144;
	const int BACKGROUND_HEIGHT = 256;
	
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

	// Scale the background image to the window size without affecting aspect ratio
	const double WINDOW_WIDTH_HEIGHT_RATIO = (double)BACKGROUND_WIDTH / BACKGROUND_HEIGHT;
	const int SCALED_BACKGROUND_HEIGHT = sdlParameters->dm.h;
	const int SCALED_BACKGROUND_WIDTH = SCALED_BACKGROUND_HEIGHT * WINDOW_WIDTH_HEIGHT_RATIO;
	const int SCALED_BACKGROUND_AREA = SCALED_BACKGROUND_WIDTH * SCALED_BACKGROUND_HEIGHT;

	int backgroundImageCount;

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
	backgroundImageCount = (double)WINDOW_AREA / SCALED_BACKGROUND_AREA;
	backgroundImageCount++;
	gameResources->backgroundSpriteCount = backgroundImageCount;

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
			sdlParameters->dm.w,
			sdlParameters->dm.h);
	}
	if (DEBUG) printf("[DEBUG INFO] Background sprites ready.\n");

	// Set the srcRect and destRect and default values for flappy bird logo from the tilemap
	// Fit the flappy bird logo to the top 1/3 of the area of the screen
	gameResources->flappyBirdLogo = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->flappyBirdLogo)
	{
		printf("[ERROR] Memory allocation failed for flappy bird logo!\n");
		return -1;
	}
	gameResources->flappyBirdLogo->name = "Flappy bird logo";
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
	ScaleSpriteToFitOnArea(gameResources->flappyBirdLogo, 0, 0, sdlParameters->dm.w, (double)sdlParameters->dm.h / 3);
	CenterSpriteHorizontallyOnScreen(gameResources->flappyBirdLogo, sdlParameters);
	ScaleSpriteInPlaceByFactor(gameResources->flappyBirdLogo, 0.8);
	if (DEBUG) printf("[DEBUG INFO] Flappy bird logo ready.\n");

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
	gameResources->playButton->destRect.y = ((double)sdlParameters->dm.h / 2) + PLAY_BUTTON_HEIGHT;
	gameResources->playButton->destRect.w = (double)sdlParameters->dm.w / 2;
	gameResources->playButton->destRect.h = (double)sdlParameters->dm.h / 2;
	ScaleSpriteInPlaceByFactor(gameResources->playButton, 0.5);
	if (DEBUG) printf("[DEBUG INFO] Play button ready.\n");

	// Set the srcRect and destRect and default values for leaderbloard button from the tilemap
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
	gameResources->leaderboardButton->destRect.x = (double)sdlParameters->dm.w / 2;
	gameResources->leaderboardButton->destRect.y = ((double)sdlParameters->dm.h / 2) + LEADERBOARD_BUTTON_HEIGHT;
	gameResources->leaderboardButton->destRect.w = (double)sdlParameters->dm.w / 2;
	gameResources->leaderboardButton->destRect.h = (double)sdlParameters->dm.h / 2;
	ScaleSpriteInPlaceByFactor(gameResources->leaderboardButton, 0.5);
	if (DEBUG) printf("[DEBUG INFO] Leaderboard button ready.\n");

	return 0;
}

void UnloadGameResources(GameResources* gameResources)
{
	// Deallocate memory for background sprites 
	if (gameResources->backgroundSpriteArray)
	{
		free(gameResources->backgroundSpriteArray);
		gameResources->backgroundSpriteArray = NULL;
		if (DEBUG) printf("[DEBUG INFO] Background sprites unloaded.\n");
	}

	// Deallocate memory for flappy bird logo
	if (gameResources->flappyBirdLogo)
	{
		free(gameResources->flappyBirdLogo);
		gameResources->flappyBirdLogo = NULL;
		if (DEBUG) printf("[DEBUG INFO] Flappy bird logo unloaded.\n");
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

int DestroyText(Text* text)
{
	if (!text)
	{
		printf("Cannot destroy empty Text!\n");
		return -1;
	}

	if (DEBUG) printf("[DEBUG INFO] Destroying Text '%s'.\n", text->text);
	if (text->textSurface)
	{
		SDL_FreeSurface(text->textSurface);
	}
	if (text->textTexture)
	{
		SDL_DestroyTexture(text->textTexture);
	}
	if (text)
	{
		free(text);
	}
	if (DEBUG) printf("[DEBUG INFO] Text destroyed.\n");
	
	return 0;
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
