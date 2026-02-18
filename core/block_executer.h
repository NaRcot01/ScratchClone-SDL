//
// Created by jack on 2/18/2026.
//

#pragma once
#include "../ui/sprite.h"
#include <vector>
#include <string>


struct ScriptState {
    int sprite_index;
    int script_index;
    bool isActive = true;
    int pc = 0;
    double waitTimer = 0.0;
    std::map<int, int> loopStack;
};


void preprocessScript(std::vector<Block>& v);
void updateScript(ScriptState& state, Sprite& sprite, double deltaTime, std::vector<std::string>& logs);