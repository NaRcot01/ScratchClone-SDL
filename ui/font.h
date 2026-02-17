//
// Created by jack on 2/5/2026.
//

#pragma once

#include <SDL2/SDL.h>
#include <string>

struct _TTF_Font;
typedef struct _TTF_Font TTF_Font;

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font,std::string text, SDL_Color color);
void drawText(SDL_Renderer* renderer, TTF_Font* font,std::string text,int x, int y, SDL_Color color);
TTF_Font* loadFont();
void my_TTF_SizeUTF8(TTF_Font *Font, const char *str, int *input_w, int *input_h);