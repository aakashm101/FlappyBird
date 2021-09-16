#pragma once
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct _SdlParameters SdlParameters;
typedef struct _GameResources GameResources;
typedef struct _Text Text;
typedef struct _Sprite Sprite;
typedef struct _PillarPair PillarPair;

extern int DEBUG;

typedef enum _GameState 
{ 
	GAME_MAIN_MENU,		// The user is in main menu
	GAME_LEADERBOARD,	// The user is viewing leaderboards
	GAME_RUNNING,		// The user is playing the game
	GAME_PAUSED,		// The game is paused
	GAME_OVER,			// The game is over
} GameState;

typedef enum _ResetParameters
{
	RESET_DEFAULT = 0b0000,		// Use defaults (Do not reset background and floor).
	RESET_BACKGROUND = 0b0001,	// Reset background (especially xPos and yPos of the destRect)
	RESET_FLOOR = 0b0010,		// Reset floor (especially xPos and yPos of the destRect)
} ResetParameters;

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

typedef struct _GameResources		// (Assigned when LoadResources() is called)
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
	int pillarPairCount;			// Used to store the number of pillar pairs
	int pillarPairsLeftEndIndex;	// Used to store the index of the pillar image which is currently at the left end of the screen (Used for efficiently performing parallax effect)
	int pillarPairsRightEndIndex;	// Used to store the index of the pllar image which is currently at the right end of the screen (Used for efficiently performing parallax effect)
	int distanceBetweenPillars;		// Distance between pillars (in pixels)
	int pillarCapitalHeight;		// Height of the pillar capital (in pixels)
	int pillarPairMinimumSpacing;	// Used to store the minimum spacing between two pillars
	int parallaxGlobalXTranslation;	// Sprites will use this value for moving multiple elements on the screen at a same rate while performing parallax effect
	int gameScore;					// For crossing each pillar pair successfully, add one point
	GameState gameState;			// Stores the game state (Useful to switch between states like main menu, gameplay etc)
	SDL_Texture* tileMap;			// Pointer to the tilemap sprite
	Sprite* backgroundSpriteArray;	// Pointer to the array of background sprites
	Sprite* floorSpriteArray;		// Pointer to the array of floor sprites
	Sprite* flappyBirdLogo;			// Pointer to flappy bird logo sprite
	Sprite* playButton;				// Pointer to play button sprite
	Sprite* leaderboardButton;		// Pointer to leaderboard button sprite
	Sprite* menuButton;				// Pointer to menu button sprite
	Sprite* getReady;				// Pointer to sprite with text 'Get Ready' sprite
	Sprite* gameOver;				// Pointer to sprite with text 'Game Over' sprite
	Sprite* bird;					// Pointer to bird sprite
	Sprite** bigNumberSpriteArray;	// Pointer to array of number images (Big size)
	Text* comingSoonText;			// Pointer to 'Coming soon' text
	PillarPair* pillarPairs;		// Pointer to pillar pairs (Pair of top and bottom pillar sprites)
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
	const char* name;			// Used to identify the sprite
	int angle;					// Angle in degrees
	int xTranslation;			// The amount of pixels to move the sprite along the X-axis
	int yTranslation;			// The amount of pixels to move the sprite along the Y-axis
	int gravity;				// This value will be added to yTranslation of the sprite every frame
	int lift;					// This value will be subtracted from yTranslation of the sprite every frame
	int maxGravity;				// This value is used to limit the gravity value
	int maxLift;				// This value is used to limit the lift value
	SDL_Rect srcRect;			// The source rect which contains coordinates, width and height of the tilemap from which the pixels will be copied from and rendered on the renderer
	SDL_Rect destRect;			// The destination rect which contains coordinates, width and height of the sprite which will be used while rendering it
}Sprite;

typedef struct _PillarPair
{
	Sprite* topPillar;		// Pointer to the sprite of the top pillar
	Sprite* bottomPillar;	// Pointer to the sprite of the bottom pillar
} PillarPair;