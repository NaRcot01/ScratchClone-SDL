//
// Created by jack on 2/19/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include <string>

struct Backdrop {
    SDL_Texture* texture;
    std::string name;
    std::string file_path;
};