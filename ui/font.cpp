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

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    return texture;

}

TTF_Font* loadFont(){
    if(TTF_Init() == -1){
        // log : font can not load !
        return 0;
    }
    TTF_Font* font = TTF_OpenFont((ASSETS_PATH + std::string("font/font.ttf")).c_str(),16);
    std::cout << ASSETS_PATH + std::string("font/font.ttf");
    return font;
}