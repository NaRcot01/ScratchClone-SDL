//
// Created by jack on 1/31/2026.
//
#include "stage.h"
#include "../core/config.h"

void drawStage(SDL_Renderer *renderer, const Stage *stage) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &stage->rect);

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderFillRect(renderer, &stage->rect);
}

void initStage(Stage *stage) {
    stage->rect.x = windowConfig.width * 0.65;
    stage->rect.y = windowConfig.height * 0.04;
    stage->rect.w = windowConfig.width * 0.35;
    stage->rect.h = windowConfig.height * 0.96;
}

void resizeStage(Stage *stage) {
    stage->rect.x = windowConfig.width * 0.65;
    stage->rect.y = windowConfig.height * 0.04;
    stage->rect.w = windowConfig.width * 0.35;
    stage->rect.h = windowConfig.height * 0.96;
}


bool isInsideStage(int x, int y, const Stage &stage) {
    SDL_Point point = {x, y};
    return SDL_PointInRect(&point, &stage.rect);
}