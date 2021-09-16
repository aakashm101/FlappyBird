#pragma once
#include "global.h"

// Used for loading and unloading tilemap, sprites
int LoadGameResources(GameResources* gameResources, const SdlParameters* const sdlParameters);
void ResetGameResourceParameters(ResetParameters resetParameters, GameResources* gameResources, SdlParameters* sdlParameters);
void UnloadGameResources(GameResources* gameResources);
static Text* CreateText(const char* text, const int textSize, const SdlParameters* const sdlParameters, SDL_Color fontColor);
static void DestroyText(Text* text);
static PillarPair* CreatePillarPair(GameResources* gameResources, const SdlParameters* const sdlParameters, int pairCount);
static void SetRandomPillarHeight(PillarPair* pillarPair, GameResources* gameResources, const SdlParameters* const sdlParameters);
static void DestroyPillarPair(PillarPair* pillarPair);

// Used for performing centering and scaling operations on sprites
void CenterSpriteOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters);
void CenterSpriteHorizontallyOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters);
void CenterSpriteVerticallyOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters);
void ScaleSpriteToFitOnArea(Sprite* sprite, int destX, int destY, int destWidth, int destHeight);
void ScaleSpriteInPlaceByFactor(Sprite* sprite, float factor);

// Used for performing parallax effect operations on sprites (For backgrounds and floor)
void ParallaxEffect(Sprite* sprites, int spriteCount, int* const leftEndIndex, int* const rightEndIndex);