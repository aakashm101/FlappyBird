#pragma once
#include "global.h"

int LoadGameResources(GameResources* gameResources, const SdlParameters* const sdlParameters);
void UnloadGameResources(GameResources* gameResources);
Text* CreateText(const char* text, int textSize, const SdlParameters* const sdlParameters);
int DestroyText(Text* text);
void CenterSpriteOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters);
void CenterSpriteHorizontallyOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters);
void CenterSpriteVerticallyOnScreen(Sprite* sprite, const SdlParameters* const sdlParameters);