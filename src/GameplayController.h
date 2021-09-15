#pragma once
#include <stdbool.h>
#include "global.h"

void HandleGameplay(SdlParameters* sdlParameters, GameResources* gameResources);
static bool CheckSpriteCollision(Sprite* sprite1, Sprite* sprite2);