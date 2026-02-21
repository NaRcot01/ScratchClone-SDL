//
// Created by jack on 2/16/2026.
//

#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "font.h"

struct LibraryPanel {
    SDL_Rect rect;
    SDL_Rect closeBtnRect;
    SDL_Texture *closeBtnTexture;

    std::vector<SDL_Texture *> itemTextures;
    std::vector<SDL_Rect> itemRects;
    std::vector<std::string> itemPaths;
};


void initLibraryPanel(LibraryPanel *panel, SDL_Renderer *renderer, const std::vector<std::string> &files);

void drawLibraryPanel(SDL_Renderer *renderer, LibraryPanel *panel, TTF_Font* font);

void addItemToLibraryPanel(LibraryPanel *panel, SDL_Renderer *renderer, std::string filePath);

void cleanupLibraryPanel(LibraryPanel *panel);