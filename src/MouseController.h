#pragma once
#include <stdbool.h>
#include "global.h"

void HandleMouseEvents(SdlParameters* sdlParameters, GameResources* gameResources);
bool clickedOnSprite(const Sprite* const sprite, int mouseX, int mouseY);