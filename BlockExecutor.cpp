//
// Created by Amir on 1/30/2026.
//
#include "BLOCK.h"
#include "Sprite.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <map>
#include <thread>
#include <chrono>

using namespace std;
const double pi = 3.141592653589;

double to_rad(double deg){
    return deg * pi / 180.0;
}

std::string getBlockName(BlockType type){
    switch (type) {
        case BlockType::OP_EQUAL :
            return "OP_EQUAL";
        case BlockType::OP_GREATER:
            return "OP_GREATER";
        case BlockType::MOVE:
            return "MOVE";
        case BlockType::TURN_RIGHT:
            return "TURN_RIGHT";
        case BlockType::TURN_LEFT:
            return "TURN_LEFT";
        case BlockType::GO_TO_XY:
            return "GO_TO_XY";
        case BlockType::SAY:
            return "SAY";
        case BlockType::SHOW:
            return "SHOW";
        case BlockType::HIDE:
            return "HIDE";
        case BlockType::WAIT:
            return "WAIT";
        case BlockType::REPEAT:
            return "REPEAT";
        case BlockType::CHANGE_X:
            return "CHANGE_X";
        case BlockType::CHANGE_Y:
            return "CHANGE_Y";
        case BlockType::SET_SIZE:
            return "SET_SIZE";
        case BlockType:: RAND :
            return "RAND";
        case BlockType:: OP_LESS :
            return "OP_LESS";
        case BlockType:: OP_MUL :
            return "OP_MUL";
        case BlockType:: OP_DIV :
            return "OP_DIV";
        case BlockType:: OP_SUB :
            return "OP_SUB";
        case BlockType::SET_VAR:
            return "SET_VAR";
        case BlockType::CHANGE_VAR:
            return "CHANGE_VAR";
        default:
            return "UNKNOWN";
    }
}


void preprocessScript(std::vector <Block>& v){
    std::vector<int > st;
    for (int i=0 ; i< v.size() ; ++i){
        BlockType t =v[i].type ;
        if(t == BlockType::IF || t == BlockType:: REPEAT || t == BlockType:: FOREVER){
            st.push_back(i);
        }
        else if(t == BlockType::ELSE ){
            if(!st.empty()){
                int idx = st.back();
                st.pop_back();
                v[idx].jumpToIndex = i +1 ;
                st.push_back(i);
            }
        } else if (t == BlockType::END_IF){
            if(!st.empty()){
                int idx = st.back();
                st.pop_back();
                v[idx].jumpToIndex = i;
            }
        } else if (t == BlockType::END_REPEAT){
            if(!st.empty()){
                int idx = st.back();
                st.pop_back();
                v[i].jumpToIndex = idx;
                v[idx].jumpToIndex = i;
            }
        }
    }
}

void executeInstantBlock (const Block &block , Sprite & sprite  ){
    switch (block.type) {
        case BlockType::MOVE:{
            double steps = 0;
            if(!block.parameters.empty()){
                steps = block.parameters[0];
            }
            double rad = (90.0 - sprite.direction) * (pi / 180.0);
            double  dx = steps * std::cos(rad);
            double dy = steps * std::sin(rad);

            sprite.x +=dx ;
            sprite.y +=dy;

            break;

        }
        case BlockType::TURN_LEFT:{
            double degree = 15;
            if(!block.parameters.empty()) {
                degree = block.parameters[0];
            }
            sprite.direction -= degree;
            break;
        }
        case BlockType::TURN_RIGHT:{
            double degree = 15;
            if(!block.parameters.empty()) {
                degree = block.parameters[0];
            }
            sprite.direction += degree;
            break;
        }
        case BlockType::CHANGE_X:{
            if(!block.parameters.empty()){
                sprite.x += block.parameters[0];
            }
            break;
        }
        case BlockType::CHANGE_Y:{
            if(!block.parameters.empty()){
                sprite.y += block.parameters[0];
            }
            break;
        }
        case BlockType::GO_TO_XY:{
            if(block.parameters.size() >= 2){
                sprite.x = block.parameters[0];
                sprite.y = block.parameters[1];
            }
            break;
        }
        case BlockType::HIDE:{
            sprite.isVisible = false;
            break;
        }
        case BlockType::SHOW:{
            sprite.isVisible = true;
            break;
        }
        case BlockType::SET_SIZE:{
            if(!block.parameters.empty()){
                sprite.size = block.parameters[0];
            }
            break;
        }



        default:
            break;
    }
}

void updateScript(Script &script , Sprite &sprite , double deltaTime , vector<string> &logs){
    if (!script.isActive || script.pc >= script.blocks.size()) {
        script.isActive = false;
        return;
    }

    const Block &currentBlock = script.blocks[script.pc];

    double oldX = sprite.x;
    double oldY = sprite.y;
    double oldDir = sprite.direction;
    bool oldVis = sprite.isVisible;
    bool executed = false;

    if (currentBlock.type == BlockType::WAIT) {
        if (script.waitTimer <= 0.0) {
            script.waitTimer = currentBlock.parameters.empty() ? 1.0 : currentBlock.parameters[0];
            logs.push_back("[WAIT] Started waiting for " + to_string(script.waitTimer) + "s");
        }
        script.waitTimer -= deltaTime;

        if (script.waitTimer <= 0.0) {
            script.pc++;
            script.waitTimer = 0.0;
        }
        return;
    }
    else if (currentBlock.type == BlockType::REPEAT) {
        if (script.loopStack.find(script.pc) == script.loopStack.end()) {
            int count = currentBlock.parameters.empty() ? 10 : (int)currentBlock.parameters[0];
            script.loopStack[script.pc] = count;
        }
        script.pc++;
    }
    else if (currentBlock.type == BlockType::END_REPEAT) {
        int startLine = currentBlock.jumpToIndex;
        if (script.loopStack[startLine] > 1) {
            script.loopStack[startLine]--;
            script.pc = startLine + 1;
        } else {
            script.loopStack.erase(startLine);
            script.pc++;
        }
    }
    else if (currentBlock.type == BlockType::IF) {
        bool condition = false;
        if (!currentBlock.parameters.empty()) condition = (currentBlock.parameters[0] != 0);

        if (condition) script.pc++;
        else script.pc = currentBlock.jumpToIndex;
    }
    else if (currentBlock.type == BlockType::ELSE) {
        script.pc = currentBlock.jumpToIndex;
    }
    else if (currentBlock.type == BlockType::END_IF) {
        script.pc++;
    }
    else if (currentBlock.type == BlockType::FOREVER) {
        script.pc = currentBlock.jumpToIndex;
    }
    else {
        executeInstantBlock(currentBlock, sprite);
        script.pc++;
        executed = true;
    }

    if (executed) {
        string log_cmd = getBlockName(currentBlock.type);
        string log_detail = "";

        if (std::abs(sprite.x - oldX) > 0.001 || std::abs(sprite.y - oldY) > 0.001) {
            log_detail = " Pos: (" + to_string((int)oldX) + "," + to_string((int)oldY) + ") -> (" +
                         to_string((int)sprite.x) + "," + to_string((int)sprite.y) + ")";
        } else if (std::abs(sprite.direction - oldDir) > 0.001) {
            log_detail = " Dir: (" + to_string((int)oldDir) + " -> " + to_string((int)sprite.direction) + ")";
        } else if (sprite.isVisible != oldVis) {
            log_detail = (sprite.isVisible ? " Hidden -> Visible" : " Visible -> Hidden");
        }

        if (!log_detail.empty() || log_cmd != "UNKNOWN") {
            logs.push_back("[CMD: " + log_cmd + "]" + log_detail);
        }
    }

}
