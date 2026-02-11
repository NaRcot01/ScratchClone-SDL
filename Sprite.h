//
// Created by Amir on 1/30/2026.
//

#ifndef SCRATCHCLONE_SDL_SPRITE_H
#define SCRATCHCLONE_SDL_SPRITE_H

#include <string>
#include <vector>
#include <map>
#include "BLOCK.h"

using namespace std;

struct Script {
    vector<Block> blocks;
    int pc = 0;
    bool isActive = false;
    double waitTimer = 0.0;
    map <int , int> loopStack;
};

struct Sprite {
    std::string name;
    double x =0.0 , y=0.0 ;
    double direction = 90.0;
    bool isVisible = true;
    double size = 100.0;
    int costumeIndex = 0;

    vector <Script> scripts;
    map<string , double> variables;
};




#endif //SCRATCHCLONE_SDL_SPRITE_H
