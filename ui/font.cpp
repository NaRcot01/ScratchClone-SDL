//
// Created by jack on 2/5/2026.
//


#include "font.h"
#include <string>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "../core/config.h"

SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font, std::string text, SDL_Color color) {

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) { return nullptr; }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;

}

TTF_Font *loadFont() {
    if (TTF_Init() == -1) {
        script_logs.push_back("font can not load !");
        return 0;
    }
    TTF_Font *font = TTF_OpenFont((ASSETS_PATH + std::string("font/font.ttf")).c_str(), 16);
    return font;
}

void drawText(SDL_Renderer *renderer, TTF_Font *font, std::string text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        // log : error loading surface
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

}

void my_TTF_SizeUTF8(TTF_Font *Font, const char *str, int *input_w, int *input_h) {
    TTF_SizeUTF8(Font,str,input_w,input_h);
}