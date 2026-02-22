
//
// Created by jack on 2/18/2026.
//

//
// Created by Amir on 1/30/2026.
//
#include "block_executor.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <map>
#include <thread>
#include <chrono>
#include <random>

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
            sprite.show = false;
            break;
        }
        case BlockType::SHOW:{
            sprite.show = true;
            break;
        }
        case BlockType::SET_SIZE:{
            if(!block.parameters.empty()){
                sprite.size = block.parameters[0];
            }
            break;
        }
        case BlockType::PLAY_SOUND: {
            const std::string& sound_name_to_play = block.textParam;
            if (sound_name_to_play.empty()) break;

            for (const auto& sound : sprite.sounds) {
                if (sound.name == sound_name_to_play + ".mp3") {
                    Mix_PlayChannel(-1, sound.chunk, 0);
                    break;
                }
                else if (sound.name == sound_name_to_play + ".wav") {
                    Mix_PlayChannel(-1, sound.chunk, 0);
                    break;
                }
            }
            break;
        }
        case BlockType::GO_TO_RANDOM_POSITION: {
            unsigned seed = time(nullptr) + rand();
            std::mt19937 gen(seed);
            std::uniform_int_distribution<> distrib_x(0, 399);
            std::uniform_int_distribution<> distrib_y(0, 607);

            sprite.x = 1101 + distrib_x(gen);
            sprite.y = 193 + distrib_y(gen);
            break;
        }
        case BlockType::SET_X: {
            if (!block.parameters.empty()) {
                std::cout << "meow";
                sprite.x = 1040 + block.parameters[0];
            }
            break;
        }

        case BlockType::SET_Y: {
            if (!block.parameters.empty()) {
                sprite.y = 32 + block.parameters[0];
            }
            break;
        }

        case BlockType::CHANGE_SIZE: {
            if (!block.parameters.empty()) {
                sprite.size = block.parameters[0];
                if (sprite.size < 0) sprite.size = 0;
            }
            break;
        }
        case BlockType::SAY: {
            if (!block.textParam.empty()) {
                sprite.is_saying = true;
                sprite.say_text = block.textParam;
                sprite.say_timer = 2.0;
            }
            break;
        }


        default:
            break;
    }
}

void updateScript(ScriptState& state, Sprite& sprite, double deltaTime, std::vector<string>& logs){
    if (!state.isActive || state.pc >= sprite.scripts[state.script_index].size()) {
        state.isActive = false;
        return;
    }

    const Block &currentBlock = sprite.scripts[state.script_index][state.pc];

    double oldX = sprite.x;
    double oldY = sprite.y;
    double oldDir = sprite.direction;
    bool oldVis = sprite.show;
    bool executed = false;

    if (currentBlock.type == BlockType::WAIT) {
        if (state.waitTimer <= 0.0) {
            state.waitTimer = currentBlock.parameters.empty() ? 1.0 : currentBlock.parameters[0];
            logs.push_back("[WAIT] Started waiting for " + to_string(state.waitTimer) + "s");
        }
        state.waitTimer -= deltaTime;

        if (state.waitTimer <= 0.0) {
            state.pc++;
            state.waitTimer = 0.0;
        }
        return;
    }
    else if (currentBlock.type == BlockType::REPEAT) {
        if (state.loopStack.find(state.pc) == state.loopStack.end()) {
            int count = currentBlock.parameters.empty() ? 10 : (int)currentBlock.parameters[0];
            state.loopStack[state.pc] = count;
        }
        state.pc++;
    }
    else if (currentBlock.type == BlockType::END_REPEAT) {
        int startLine = currentBlock.jumpToIndex;
        if (state.loopStack[startLine] > 1) {
            state.loopStack[startLine]--;
            state.pc = startLine + 1;
        } else {
            state.loopStack.erase(startLine);
            state.pc++;
        }
    }
    else if (currentBlock.type == BlockType::IF) {
        bool condition = false;

        if (!currentBlock.block_parameters.empty() && currentBlock.block_parameters[0]) {
            condition = (evaluateReporter(*currentBlock.block_parameters[0], sprite) != 0.0);
        }
        else if (!currentBlock.parameters.empty()) {
            condition = (currentBlock.parameters[0] != 0);
        }
        if (condition) {
            state.pc++;
        } else {
            state.pc = currentBlock.jumpToIndex;
        }
    }
    else if (currentBlock.type == BlockType::ELSE) {
        state.pc = currentBlock.jumpToIndex;
    }
    else if (currentBlock.type == BlockType::END_IF) {
        state.pc++;
    }
    else if (currentBlock.type == BlockType::FOREVER) {
        state.pc = currentBlock.jumpToIndex;
    }
    else {
        executeInstantBlock(currentBlock, sprite);
        state.pc++;
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
        } else if (sprite.show != oldVis) {
                log_detail = (sprite.show ? " Hidden -> Visible" : " Visible -> Hidden");
        }

        if (!log_detail.empty() || log_cmd != "UNKNOWN") {
            logs.push_back("[CMD: " + log_cmd + "]" + log_detail);
        }
    }

}

double getParamValue (int param_index,const Block& reporter,const Sprite& sprite){

    if (param_index < reporter.block_parameters.size() && reporter.block_parameters[param_index]) {
        return evaluateReporter(*reporter.block_parameters[param_index], sprite);
    }
    if (param_index < reporter.parameters.size()) {
        return reporter.parameters[param_index];
    }
    return 0.0;
}

double evaluateReporter(const Block& reporter, const Sprite& sprite) {

    switch (reporter.type) {
        case BlockType::X_POSITION:
            return sprite.x;
        case BlockType::Y_POSITION:
            return sprite.y;
            // TODO: افزودن direction, size, ...

        case BlockType::GREATER_THAN: {
            double val1 = getParamValue(0,reporter,sprite);
            double val2 = getParamValue(1,reporter,sprite);
            return (val1 > val2) ? 1.0 : 0.0;
        }
            // TODO: افزودن <, =, +, -, *, /

        default:
            return 0.0;
    }
}