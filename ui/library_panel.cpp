//
// Created by jack on 2/16/2026.
//

#include "library_panel.h"
#include "../core/config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

void initLibraryPanel(SDL_Renderer *renderer, LibraryPanel *panel) {

    panel->rect.w = windowConfig.width * 0.7;
    panel->rect.h = windowConfig.height * 0.7;
    panel->rect.x = (windowConfig.width - panel->rect.w) / 2;
    panel->rect.y = (windowConfig.height - panel->rect.h) / 2;

    SDL_Surface *closeBtnSurface = IMG_Load((ASSETS_PATH + "icons/close.png").c_str());
    if (closeBtnSurface) {
        panel->closeBtnTexture = SDL_CreateTextureFromSurface(renderer, closeBtnSurface);
        SDL_FreeSurface(closeBtnSurface);
    }
    panel->closeBtnRect = {
            panel->rect.x + panel->rect.w - 40,
            panel->rect.y + 10,
            30, 30
    };
    int padding = 20;
    int item_size = 100;
    int items_per_row = 5;
    int start_x = panel->rect.x + padding;
    int start_y = panel->rect.y + 50;

    for (int i = 0; i < libraryFiles.size(); i++) {
        std::string fullPath = ASSETS_PATH + libraryFiles[i];
        SDL_Surface *surface = IMG_Load(fullPath.c_str());
        if (surface) {
            panel->itemTextures.push_back(SDL_CreateTextureFromSurface(renderer, surface));
            SDL_FreeSurface(surface);
            panel->itemPaths.push_back(fullPath);

            int row = i / items_per_row;
            int col = i % items_per_row;
            SDL_Rect itemRect = {
                    start_x + col * (item_size + padding),
                    start_y + row * (item_size + padding),
                    item_size,
                    item_size
            };
            panel->itemRects.push_back(itemRect);
        } else {
            // log : error while loading library. pictures are missing
        }
    }
}

void drawLibraryPanel(SDL_Renderer *renderer, LibraryPanel *panel) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &panel->rect);

    if (panel->closeBtnTexture) {
        SDL_RenderCopy(renderer, panel->closeBtnTexture, nullptr, &panel->closeBtnRect);
    }

    for (int i = 0; i < panel->itemTextures.size(); i++) {
        SDL_RenderCopy(renderer,panel->itemTextures[i], nullptr,&panel->itemRects[i]);
    }
}

void addItemToLibraryPanel(LibraryPanel* panel, SDL_Renderer* renderer, std::string filePath){
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        // log : Could not load new library item: + filePath
        return;
    }

    panel->itemTextures.push_back(SDL_CreateTextureFromSurface(renderer, surface));
    SDL_FreeSurface(surface);
    panel->itemPaths.push_back(filePath);


    int padding = 20;
    int item_size = 100;
    int items_per_row = 5;
    int start_x = panel->rect.x + padding;
    int start_y = panel->rect.y + 50;

    size_t new_item_index = panel->itemRects.size();
    int row = new_item_index / items_per_row;
    int col = new_item_index % items_per_row;

    SDL_Rect item_r = {
            start_x + col * (item_size + padding),
            start_y + row * (item_size + padding),
            item_size,
            item_size
    };
    panel->itemRects.push_back(item_r);
}