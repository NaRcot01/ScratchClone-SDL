//
// Created by jack on 2/2/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include "stage.h"


struct Sprite{
    SDL_Rect rect;
    bool selected;
    bool dragging;
    int diff_x_mouse;
    int diff_y_mouse;
};

void initSprite(Sprite& sprite, const Stage* stage);
void drawSprite(SDL_Renderer* renderer, const Sprite* sprite);
bool isSpriteClicked(int x, int y, const Sprite* sprite);
void moveSprite(Sprite* sprite, int x, int y, const Stage* stage, bool differential = false);
