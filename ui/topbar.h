//
// Created by jack on 2/5/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include "topbar_button.h"
#include "font.h"

#define TOPBAR_MAX_BUTTONS 10

struct TopBar{
    SDL_Rect rect;
    TopBarButton buttons[TOPBAR_MAX_BUTTONS];
    int buttonCount;
};

void initTopBar(TopBar* bar);
void drawTopBar(SDL_Renderer* renderer, TopBar* bar,TTF_Font* font);