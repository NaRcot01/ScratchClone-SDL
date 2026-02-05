//
// Created by jack on 2/5/2026.
//

#pragma once

#include <SDL2/SDL.h>
#include <string>

struct _TTF_Font;
typedef struct _TTF_Font TTF_Font;

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font,std::string text, SDL_Color color);
TTF_Font* loadFont();