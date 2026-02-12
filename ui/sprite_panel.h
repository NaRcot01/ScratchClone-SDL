//
// Created by jack on 2/4/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include "sprite.h"

struct SpritePanel{
    SDL_Rect rect;

    SDL_Rect buttonRects[4];
    SDL_Texture* buttonTextures[4];
};


void initSpritePanel(SpritePanel* panel, SDL_Renderer* renderer);
void drawSpritePanelBase(SDL_Renderer *renderer, const SpritePanel *panel);
void drawSpritePanelItem(SDL_Renderer *renderer, const SpritePanel *panel, const Sprite *sprite, int index, int panelSelectedIndex);
