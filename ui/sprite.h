//
// Created by jack on 2/2/2026.
//

#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "stage.h"


struct Sprite{
    SDL_Rect rect;
    SDL_Texture* texture;
    std::string name;
    bool selected;
    bool dragging;
    int diff_x_mouse;
    int diff_y_mouse;
    int rotation;
    int size;
    bool show;
};

void initSprite(Sprite& sprite, const Stage* stage);
void drawSprite(SDL_Renderer* renderer, const Sprite* sprite);
void spriteValidate(Sprite *sprite, const Stage *stage);
bool isSpriteClicked(int x, int y, const Sprite* sprite);
void moveSprite(Sprite* sprite, int x, int y, const Stage* stage, bool differential = false);
bool loadSpriteTexture(SDL_Renderer* renderer, Sprite& sprite, std::string path);
void updateSpriteSize(Sprite *sprite);