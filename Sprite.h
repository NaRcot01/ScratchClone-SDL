//
// Created by Amir on 1/30/2026.
//

#ifndef SCRATCHCLONE_SDL_SPRITE_H
#define SCRATCHCLONE_SDL_SPRITE_H

#include <string>
#include <vector>

struct Sprite {
    std::string name;
    double x =0.0 , y=0.0 ;
    double direction = 90.0;
    bool isVisible = true;
    double size = 100.0;
    int costumeIndex = 0;
};




#endif //SCRATCHCLONE_SDL_SPRITE_H
