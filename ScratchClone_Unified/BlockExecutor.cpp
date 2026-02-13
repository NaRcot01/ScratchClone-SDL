//
// Created by Amir on 1/30/2026.
//
// updated by mehdi on 2/13/2026.
// adapting logic to the new unified sprite.

#include "ui/BLOCK.h"
#include "ui/sprite.h"
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
	case BlockType::FOREVER:
            return "FOREVER";
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
	case BlockType::OP_ADD:
            return "OP_ADD";
	case BlockType::END_REPEAT:
            return "END_REPEAT";
	case BlockType::END_IF:
            return "END_IF";
	case BlockType::IF:
            return "IF";
	case BlockType::ELSE:
            return "ELSE";
	case BlockType::ON_FLAG_CLICKED:
            return "ON_FLAG_CLICKED";
	case BlockType::CHANGE_X:
            return "CHANGE_X";
	case BlockType::CHANGE_Y:
            return "CHANGE_Y";
	case BlockType::BROADCAST:
            return "BROADCAST";
        default:
            return "UNKNOWN";
    }
}


void preprocessScript(std::vector <Block>& v){
    std::vector<int > st;
    for (int i=0 ; i< (int)v.size() ; ++i){
        BlockType t =v[i].type ;
        if(t == BlockType::IF || t == BlockType:: REPEAT || t == BlockType:: FOREVER){
            st.push_back(i);
        }
        else if(t == BlockType::ELSE ){
            if(!st.empty()){
                int idx = st.back();
                st.pop_back();
                v[idx].jumpToIndex = i+1 ;
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
            double steps = 0.0;
            if(!block.parameters.empty()){
                steps = block.parameters[0];
            }
            double rad = (90.0 - sprite.direction) * (pi / 180.0);
            double  dx = steps * std::cos(rad);
            double dy = steps * std::sin(rad);

            sprite.exact_x +=dx ; //updated precise position x
            sprite.exact_y -=dy;  //updated precise position y ,added minus sign
	    
            sprite.rect.x = (int)sprite.exact_x; //syncing to visual rect
	    sprite.rect.y = (int)sprite.exact_y; //syncing to cisual rect
            break;

        }
        case BlockType::TURN_LEFT:{
            double degree = 15.0;
            if(!block.parameters.empty()) {
                degree = block.parameters[0];
            }
            sprite.direction -= degree;
	    sprite.rotation = (int)(sprite.direction - 90.0); //added rotation
            break;
        }
        case BlockType::TURN_RIGHT:{
            double degree = 15;
            if(!block.parameters.empty()) {
                degree = block.parameters[0];
            }
            sprite.direction += degree;
	    sprite.rotation = (int)(sprite.direction - 90.0); //added rotation
            break;
        }
        case BlockType::CHANGE_X:{
            if(!block.parameters.empty()){
                sprite.exact_x += block.parameters[0]; //updated precise position x

		sprite.rect.x = (int)sprite.exact_x; //syncing to visual rect
            }
            break;
        }
        case BlockType::CHANGE_Y:{
            if(!block.parameters.empty()){
                sprite.exact_y -= block.parameters[0]; //updated precise position y

		sprite.rect.y = (int)sprite.exact_y; //syncing to visual rect		
            }
            break;
        }
        case BlockType::GO_TO_XY:{
            if(block.parameters.size() >= 2){
                sprite.exact_x = block.parameters[0]; //updated precise position x
                sprite.exact_y = block.parameters[1]; //updated precise position y

		sprite.rect.x = (int)sprite.exact_x; //syncing to visual rect
		sprite.rect.y = (int)sprite.exact_y; //syncing to visual rect
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
		updateSpriteSize(&sprite);
            }
            break;
        }
	case BlockType::SAY: {
		if (!block.textParam.empty()) {
			sprite.name = block.textParam;
		}
		break;
	}
	case BlockType::SET_VAR: {
		if (!block.textParam.empty() && !block.parameters.empty()) {
			sprite.variables[block.textParam] = block.parameters[0];
		}
		break;
	}
	case BlockType::CHANGE_VAR: {
		if (!block.textParam.empty() && !block.parameters.empty()) {
			double delta = block.parameters[0];
			sprite.variables[block.textParam] +=delta;
		}
		break;
	}

	case BlockType::OP_ADD:
	case BlockType::OP_SUB:
	case BlockType::OP_MUL:
	case BlockType::OP_DIV:
	case BlockType::OP_EQUAL:
	case BlockType::OP_LESS:
	case BlockType::OP_GREATER:
	case BlockType::RAND: {
	break;
	}

	case BlockType::ON_FLAG_CLICKED:
	case BlockType::BROADCAST: {
	break;
	}

	case BlockType::WAIT:
	case BlockType::REPEAT:
	case BlockType::END_REPEAT:
	case BlockType::FOREVER:
	case BlockType::IF:
	case BlockType::ELSE:
	case BlockType::END_IF: {
	break;
	}

        default:
            break;
    }
}

void updateScript(Script &script , Sprite &sprite , double deltaTime , vector<string> &logs){
    if (!script.isActive || script.pc >= (int)script.blocks.size() || script.pc<0) {
        script.isActive = false;
        return;
    }
    
    script.instructionsThisFrame = 0;
    const int MAX_STEPS_PER_FRAME = 32;
    int steps = 0;
	
	while (steps < MAX_STEPS_PER_FRAME && script.isActive && script.pc >= 0 && script.pc < (int)script.blocks.size()) {

    const Block &currentBlock = script.blocks[script.pc];

    double oldX = sprite.exact_x;
    double oldY = sprite.exact_y;
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
	    if (count<0) count=0;
            script.loopStack[script.pc] = count;
        }
        script.pc++;
    }
    else if (currentBlock.type == BlockType::END_REPEAT) {
        int startLine = currentBlock.jumpToIndex;
	auto it = script.loopStack.find(startLine);
        if (it != script.loopStack.end()) {
            if (it->second > 1) {
		it->second -= 1;
		script.pc = startLine + 1;
        } else {
            script.loopStack.erase(it);
            script.pc++;
        }
    } else { script.pc++;}}
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
	script.instructionsThisFrame++;
	if (script.instructionsThisFrame>1000){
		logs.push_back("[ERROR] Watchdog: too many instructions in one frame, stopping script.");
		script.isActive = false;
		return;
		
	}

        std::string log_cmd = getBlockName(currentBlock.type);
        std::string log_detail;

        if (std::abs(sprite.exact_x - oldX) > 0.001 || std::abs(sprite.exact_y - oldY) > 0.001) {
            log_detail = " Pos: (" + to_string((int)oldX) + "," + to_string((int)oldY) + ") -> (" +
                         to_string((int)sprite.exact_x) + "," + to_string((int)sprite.exact_y) + ")";
        } else if (std::abs(sprite.direction - oldDir) > 0.001) {
            log_detail = " Dir: (" + to_string((int)oldDir) + " -> " + to_string((int)sprite.direction) + ")";
        } else if (sprite.isVisible != oldVis) {
            log_detail = (sprite.isVisible ? " Hidden -> Visible" : " Visible -> Hidden");
        }

        if (!log_detail.empty() || log_cmd != "UNKNOWN") {
            logs.push_back("[CMD: " + log_cmd + "]" + log_detail);
            }
    	}
steps++; 
    }

}
