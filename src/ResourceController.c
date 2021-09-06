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

	// Scale the background image to the window size without affecting aspect ratio
	const double WINDOW_WIDTH_HEIGHT_RATIO = (double)BACKGROUND_WIDTH / BACKGROUND_HEIGHT;
	const int SCALED_BACKGROUND_HEIGHT = sdlParameters->dm.h;
	const int SCALED_BACKGROUND_WIDTH = SCALED_BACKGROUND_HEIGHT * WINDOW_WIDTH_HEIGHT_RATIO;
	const int SCALED_BACKGROUND_AREA = SCALED_BACKGROUND_WIDTH * SCALED_BACKGROUND_HEIGHT;
	
	// Used to store how many background images will fit inside the window (Using multiple scrolling background images to create parallax effect)
	int backgroundImageCount = (double)WINDOW_AREA / SCALED_BACKGROUND_AREA;

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

	// Use one more background image (It can be partially outside the current window) to create the parallax effect
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
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.w = SCALED_BACKGROUND_WIDTH;
		gameResources->backgroundSpriteArray[backgroundSpriteIndex].destRect.h = SCALED_BACKGROUND_HEIGHT;
	}
	if (DEBUG) printf("[DEBUG INFO] Background sprites ready.\n");

	// Set the srcRect and destRect and default values for flappy bird logo from the tilemap
	gameResources->flappyBirdLogo = (Sprite*)malloc(sizeof(Sprite));
	if (!gameResources->flappyBirdLogo)
	{
		printf("[ERROR] Memory allocation failed for flappy bird logo!\n");
		return -1;
	}
	gameResources->flappyBirdLogo->name = "flappy bird logo";
	gameResources->flappyBirdLogo->angle = 0;
	gameResources->flappyBirdLogo->xTranslation = 0;
	gameResources->flappyBirdLogo->yTranslation = 0;
	gameResources->flappyBirdLogo->srcRect.x = 350;
	gameResources->flappyBirdLogo->srcRect.y = 90;
	gameResources->flappyBirdLogo->srcRect.w = 92;
	gameResources->flappyBirdLogo->srcRect.h = 26;
	gameResources->flappyBirdLogo->destRect.x = 0;
	gameResources->flappyBirdLogo->destRect.y = 0;
	gameResources->flappyBirdLogo->destRect.w = 92 * 5;
	gameResources->flappyBirdLogo->destRect.h = 26 * 5;
	if (DEBUG) printf("[DEBUG INFO] Flappy bird logo ready.\n");

	CenterSpriteHorizontallyOnScreen(gameResources->flappyBirdLogo, sdlParameters);

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

Text* CreateText(const char* text, int textSize, const SdlParameters* const sdlParameters)
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
	newText->fontColor.r = 0x00;
	newText->fontColor.g = 0x00;
	newText->fontColor.b = 0x00;
	newText->fontColor.a = 0xFF;
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
	newText->textSurface = TTF_RenderText_Blended(font, newText->text, newText->fontColor);
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
	return;
}

void CenterSpriteVerticallyOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters)
{
	if (!sprite)
	{
		printf("[ERROR] Cannot center empty sprite!\n");
		return;
	}
	sprite->destRect.y = ((double)sdlParameters->dm.h / 2) - ((double)sprite->destRect.h / 2);;
}
