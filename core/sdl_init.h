//
// Created by jack on 1/31/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include <iostream>
bool initSDL(SDL_Window* &window, SDL_Renderer* &renderer,int H, int W);
void quitSDL(SDL_Window* &window, SDL_Renderer* &renderer);