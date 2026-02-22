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

bool copyFile(std::string sourcePath, std::string destPath){
    std::ifstream source(sourcePath,std::ios::binary);
    if(!source){
        // log :
        script_logs.push_back("can not add sprite from system files to library");
        return false;
    }

    std::ofstream dest(destPath,std::ios::binary);
    dest << source.rdbuf();

    return true;
}

bool helperFunc_scriptRemover(const ScriptState& s){
    return !s.isActive;
}