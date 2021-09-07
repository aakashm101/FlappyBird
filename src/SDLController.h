#pragma once
#include "global.h"

int SDL_Start(SdlParameters* sdlParameters);
void SDL_End(SdlParameters* sdlParameters);
void CopySpriteToRenderer(Sprite* sprite, SdlParameters* const sdlParameters, const GameResources* const gameResources);