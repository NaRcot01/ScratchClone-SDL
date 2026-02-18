//
// Created by jack on 1/31/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include "../ui/sprite.h"

struct Engine{
    bool running;
    bool is_running_scripts = false;
};

void initBase(SDL_Renderer *renderer);
void engineInit(Engine &engine);
void engineHandleEvents(Engine &engine, SDL_Renderer* renderer);
void engineUpdate(Engine& engine);
void engineDraw(SDL_Renderer* renderer);
void drawSprites(SDL_Renderer *renderer);
void drawSpritePanels(SDL_Renderer *renderer);
