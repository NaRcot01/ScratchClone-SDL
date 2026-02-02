//
// Created by jack on 2/2/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include "stage.h"


struct Sprite{
    SDL_Rect rect;
    bool selected;
};

void initSprite(Sprite& sprite, const Stage* stage);
void drawSprite(SDL_Renderer* renderer, const Sprite* sprite);
bool isSpriteClicked(int x, int y, const Sprite* sprite);
void moveSprite(Sprite* sprite, int d_x, int d_y, const Stage* stage);
