//
// Created by jack on 1/31/2026.
//
#pragma once
#include <SDL2/SDL.h>

struct Stage{
    SDL_Rect rect;
};

void initStage(Stage* stage);
void resizeStage(Stage* stage);
void drawStage(SDL_Renderer* renderer, const Stage* stage);
bool isInsideStage(int x, int y, Stage& stage);
