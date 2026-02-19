//
// Created by jack on 1/31/2026.
//
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "backdrop.h"

struct Stage{
    SDL_Rect rect;

    std::vector<Backdrop> backgrounds;
    int active_background_index = -1;
};

void initStage(Stage* stage);
void resizeStage(Stage* stage);
void drawStage(SDL_Renderer* renderer, const Stage* stage);
bool isInsideStage(int x, int y, Stage& stage);
void addNewBackgroundFromFile(SDL_Renderer* renderer, Stage* stage, const char* filePath);