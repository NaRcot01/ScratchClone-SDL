//
// Created by jack on 2/22/2026.
//

#pragma once
#include <string>
#include <SDL2/SDL_mixer.h>
#include <string>


struct Sound {

    Mix_Chunk* chunk;

    std::string name;

    std::string file_path;
};

