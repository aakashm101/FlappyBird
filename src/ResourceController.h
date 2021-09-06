#pragma once
#include "global.h"

int LoadGameResources(GameResources* gameResources, SdlParameters* sdlParameters);
void UnloadGameResources(GameResources* gameResources);
Text* createText(const char* text, int textSize, SdlParameters* sdlParameters);
int destroyText(Text* text);