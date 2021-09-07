#pragma once
#include "global.h"

int LoadGameResources(GameResources* gameResources, const SdlParameters* const sdlParameters);
void UnloadGameResources(GameResources* gameResources);
Text* CreateText(const char* text, const int textSize, SdlParameters* const sdlParameters, SDL_Color fontColor);
int DestroyText(Text* text);
void CenterSpriteOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters);
void CenterSpriteHorizontallyOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters);
void CenterSpriteVerticallyOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters);
void ScaleSpriteToFitOnArea(Sprite* sprite, int destX, int destY, int destWidth, int destHeight);
void ScaleSpriteInPlaceByFactor(Sprite* sprite, float factor);