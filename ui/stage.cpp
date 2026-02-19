//
// Created by jack on 1/31/2026.
//
#include <iostream>
#include "stage.h"
#include "../core/config.h"
#include "../tools/tinyfiledialogs.h"

void drawStage(SDL_Renderer *renderer, const Stage *stage) {
    if(stage->active_background_index != -1 && stage->active_background_index < stage->backgrounds.size()){

        const Backdrop& active_background = stage->backgrounds[stage->active_background_index];
        if(active_background.texture){

            SDL_RenderCopy(renderer,active_background.texture, nullptr,&stage->rect);
        }

    }
    else{
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &stage->rect);
    }


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


bool isInsideStage(int x, int y, Stage &stage) {
    SDL_Point point = {x, y};
    return SDL_PointInRect(&point, &stage.rect);
}

void addNewBackgroundFromFile(SDL_Renderer* renderer, Stage* stage, const char* filePath) {
    if (!filePath) return;

    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        // log : unable to load the image that has been inserted as stage background.
        return;
    }

    SDL_Texture* new_backdrop_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (new_backdrop_texture) {
        std::string path_str(filePath);
        size_t last_slash = path_str.find_last_of("/\\");
        std::string name = (last_slash == std::string::npos) ? path_str : path_str.substr(last_slash + 1);

        Backdrop new_backdrop = {new_backdrop_texture, name};
        stage->backgrounds.push_back(new_backdrop);
        stage->active_background_index = stage->backgrounds.size() - 1;
        // log :  new stage background has added and set as active.
    }
}