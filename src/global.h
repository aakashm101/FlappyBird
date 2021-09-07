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
	GAME_MAIN_MENU,		// The user is in main menu
	GAME_LEADERBOARD,	// The user is viewing leaderboards
	GAME_RUNNING,		// The user is playing the game
	GAME_PAUSED,		// The game is paused
	GAME_FINISHED,		// The game is finished
} GameState;

typedef struct _SdlParameters 
{
	const char* fontPath;	// Used to store the font path 
	bool mainLoopRunning;	// This is the main boolean value which controls SDL mainloop
	int FPS;				// Frames per second
	SDL_Window* window;		// Pointer to sdl window (The renderer will render in this)
	SDL_Renderer* renderer;	// Pointer to sdl renderer
	SDL_DisplayMode dm;		// Used to fetch the display parameters (Example: resolution)
	SDL_Event event;		// Used to check keyboard and mouse events
} SdlParameters;

typedef struct _GameResources
{
	const char* tileMapPath;		// Used to store the path where the tilemap is present
	int tileMapWidth;				// Used to store the tilemap width
	int tileMapHeight;				// Used to store the tilemap height
	int backgroundSpriteCount;		// Used to store the number of background images which can fit on screen (Parallax effect need multiple backgrounds)
	int backgroundLeftEndIndex;		// Used to store the index of the background image which is currently at the left end of the screen (Used for efficiently performing parallax effect)
	int backgroundRightEndIndex;	// Used to store the index of the background image which is currently at the right end of the screen (Used for efficiently performing parallax effect)
	int floorSpriteCount;			// Used to store the number of floor images which can fit on screen (Parallax effect need multiple backgrounds)
	int floorLeftEndIndex;			// Used to store the index of the floor image which is currently at the left end of the screen (Used for efficiently performing parallax effect)
	int floorRightEndIndex;			// Used to store the index of the floor image which is currently at the right end of the screen (Used for efficiently performing parallax effect)
	GameState gameState;			// Stores the game state (Useful to switch between states like main menu, gameplay etc)
	SDL_Texture* tileMap;			// Pointer to the tilemap (Assigned when LoadResources() is called)
	Sprite* backgroundSpriteArray;	// Pointer to the array of backgrounds (Assigned when LoadResources() is called)
	Sprite* floorSpriteArray;		// Pointer to the array of floor (Assigned when LoadResources() is called)
	Sprite* flappyBirdLogo;			// Pointer to flappy bird logo (Assigned when LoadResources() is called)
	Sprite* playButton;				// Pointer to play button (Assigned when LoadResources() is called)
	Sprite* leaderboardButton;		// Pointer to leaderboard button (Assigned when LoadResources() is called)
	Text* comingSoonText;			// Pointer to 'Coming soon' text (Assigned when LoadResources() is called)
} GameResources;

typedef struct _Text 
{
	SDL_Texture* textTexture;	// The text texture will be rendered on screen created from textSurface
	SDL_Surface* textSurface;	// The surface of the texture
	SDL_Rect destRect;			// The destination rect which contains coordinates, width and height of the text which will be used while rendering it
	int textSize;				// Point size of the text (based on 72DPI)
	const char* text;			// Pointer to the actual text
}Text;

typedef struct _Sprite
{
	const char* name;	// Used to identify the sprite
	int angle;			// Angle in degrees
	int xTranslation;	// The amount of pixels to move the sprite along the X-axis
	int yTranslation;	// The amount of pixels to move the sprite along the Y-axis
	SDL_Rect srcRect;	// The source rect which contains coordinates, width and height of the tilemap from which the pixels will be copied from and rendered on the renderer
	SDL_Rect destRect;	// The destination rect which contains coordinates, width and height of the sprite which will be used while rendering it
}Sprite;