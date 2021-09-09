#pragma once
#include "global.h"

int LoadGameResources(GameResources* gameResources, const SdlParameters* const sdlParameters);
void UnloadGameResources(GameResources* gameResources);
void CenterSpriteOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters);
void CenterSpriteHorizontallyOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters);
void CenterSpriteVerticallyOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters);
void ScaleSpriteToFitOnArea(Sprite* sprite, int destX, int destY, int destWidth, int destHeight);
void ScaleSpriteInPlaceByFactor(Sprite* sprite, float factor);
Text* CreateText(const char* text, const int textSize, const SdlParameters* const sdlParameters, SDL_Color fontColor);
void DestroyText(Text* text);
PillarPair* CreatePillarPair(GameResources* gameResources, const SdlParameters* const sdlParameters, int pairCount);
void DestroyPillarPair(PillarPair* pillarPair);
void SetRandomPillarHeight(PillarPair* pillarPair, GameResources* gameResources, const SdlParameters* const sdlParameters);
void InvertSpriteVerticalForces(Sprite* sprite, int durationInMilliSeconds, SdlParameters* sdlParameters, GameResources* gameResources);
void ParallaxEffect(Sprite* sprites, int spriteCount, int* const leftEndIndex, int* const rightEndIndex);