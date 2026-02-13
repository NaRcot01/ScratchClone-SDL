//
// Created by jack on 2/5/2026.
//

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font,std::string text, SDL_Color color);
void drawText(SDL_Renderer* renderer, TTF_Font* font,std::string text,int x, int y, SDL_Color color);
TTF_Font* loadFont();