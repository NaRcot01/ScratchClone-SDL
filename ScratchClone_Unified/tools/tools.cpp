//
// Created by jack on 2/6/2026.
//
#include "tools.h"


std::string generateRandomName(int length){
    std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    static std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<> dist(0,sizeof(chars)-2);

    std::string name = "Sprite_";

    for(int i =0;i<length;i++){
        name += chars[dist(rng)];
    }
    return name;
}
