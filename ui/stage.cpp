//
// Created by jack on 1/31/2026.
//
#include "stage.h"


void drawStage(SDL_Renderer* renderer, const Stage& stage){
    SDL_SetRenderDrawColor(renderer,100,100,100,255);
    SDL_RenderFillRect(renderer, &stage.rect);

    SDL_SetRenderDrawColor(renderer,20,20,20,255);
    SDL_RenderFillRect(renderer,&stage.rect);
}


bool isInsideStage(int x, int y, const Stage& stage){
    SDL_Point point = {x,y};
    return SDL_PointInRect(&point,&stage.rect);
}