//
// Created by jack on 2/4/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include "sprite.h"

struct SpritePanel{
    SDL_Rect rect;
};


void initSpritePanel(SpritePanel* panel);
void drawSpritePanel(SDL_Renderer* renderer, const SpritePanel* panel, const Sprite* sprite);
