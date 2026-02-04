//
// Created by jack on 1/31/2026.
//

#pragma once
#include <SDL2/SDL.h>

struct Engine{
    bool running;;
};

void initBase(SDL_Renderer *renderer);
void engineInit(Engine &engine);
void engineHandleEvents(Engine &engine);
void engineUpdate();
void engineDraw(SDL_Renderer* renderer);
void drawSprites(SDL_Renderer *renderer);
void drawSpritePanels(SDL_Renderer *renderer);