#include <stdio.h>
#include <SDL2/SDL_image.h>
#include "global.h"
#include "ResourceController.h"

int LoadGameResources(GameResources* gameResources, SdlParameters* sdlParameters)
{
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

	return 0;
}

void UnloadGameResources(GameResources* gameResources)
{
	// Deallocate the memory for the tilemap
	if (gameResources->tileMap)
	{
		SDL_DestroyTexture(gameResources->tileMap);
		if (DEBUG) printf("[DEBUG INFO] Tilemap %s unloaded.\n", gameResources->tileMapPath);
	}
	return;
}

Text* createText(const char* text, int textSize, SdlParameters* sdlParameters)
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

int destroyText(Text* text)
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
