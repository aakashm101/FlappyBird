#pragma once
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct _SdlParameters SdlParameters;
typedef struct _GameResources GameResources;
typedef struct _Text Text;

//#define DEBUG	// Define it to print additional info for debugging
extern int DEBUG;

extern Text* helloWorld;

typedef enum _GameState { GAME_MAIN_MENU, GAME_RUNNING, GAME_PAUSED, GAME_FINISHED, GAME_RESTART } GameState;

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
	SDL_Texture* tileMap;
	int tileMapWidth;
	int tileMapHeight;
	GameState gameState;
} GameResources;

typedef struct _Text 
{
	SDL_Texture* textTexture;
	SDL_Surface* textSurface;
	SDL_Color fontColor;
	SDL_Rect destRect;
	int textSize;
	const char* text;
}Text;