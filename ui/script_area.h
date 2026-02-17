//
// Created by jack on 2/17/2026.
//
#pragma once
#include <SDL2/SDL.h>
#include "sprite.h"
#include "font.h"

struct ScriptArea {
    SDL_Rect rect;
    int scroll_offset_y = 0;
};

void initScriptArea(ScriptArea* area);
void drawScriptArea(SDL_Renderer* renderer, ScriptArea* area, Sprite* activeSprite, TTF_Font* font);
void drawBlock(SDL_Renderer* renderer, Block* block, TTF_Font* font);