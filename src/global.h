#pragma once
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct _SdlParameters SdlParameters;
typedef struct _GameResources GameResources;
typedef struct _Text Text;
typedef struct _Sprite Sprite;

extern int DEBUG;

typedef enum _GameState 
{ 
	GAME_MAIN_MENU, 
	GAME_RUNNING, 
	GAME_PAUSED, 
	GAME_FINISHED, 
	GAME_RESTART 
} GameState;

typedef struct _SdlParameters 
{
	const char* fontPath;
	bool mainLoopRunning;
	int FPS;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_DisplayMode dm;
	SDL_Event event;
} SdlParameters;

typedef struct _GameResources
{
	const char* tileMapPath;
	int tileMapWidth;
	int tileMapHeight;
	int backgroundSpriteCount;
	GameState gameState;
	SDL_Texture* tileMap;
	Sprite* backgroundSpriteArray;
	Sprite* flappyBirdLogo;
	Sprite* playButton;
	Sprite* leaderboardButton;
} GameResources;

typedef struct _Text 
{
	SDL_Texture* textTexture;
	SDL_Surface* textSurface;
	SDL_Rect destRect;
	int textSize;
	const char* text;
}Text;

typedef struct _Sprite
{
	const char* name;
	int angle;
	int xTranslation;
	int yTranslation;
	SDL_Rect srcRect;
	SDL_Rect destRect;
}Sprite;