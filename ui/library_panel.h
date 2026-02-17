//
// Created by jack on 2/16/2026.
//

#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

struct LibraryPanel {
    SDL_Rect rect;
    SDL_Rect closeBtnRect;
    SDL_Texture *closeBtnTexture;

    std::vector<SDL_Texture *> itemTextures;
    std::vector<SDL_Rect> itemRects;
    std::vector<std::string> itemPaths;
};


void initLibraryPanel(SDL_Renderer* renderer, LibraryPanel *panel);

void drawLibraryPanel(SDL_Renderer* renderer, LibraryPanel *panel);

void cleanupLibraryPanel(LibraryPanel *panel);

