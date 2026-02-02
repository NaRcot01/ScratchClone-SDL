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

void executeBlock (const Block &block , Sprite & sprite , int &lineNum , map <int , int> &loops){
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

        case BlockType::WAIT :{
            double seconds = block.parameters.empty() ? 1.0 : block.parameters[0];
            std::this_thread::sleep_for(chrono::milliseconds((int)(seconds * 1000)));
            break;
        }
        case BlockType::IF:{
            bool condition = false ;
            if(!block.parameters.empty())
                condition = (block.parameters[0] != 0);

            if(!condition) {
                if (block.jumpToIndex != -1) {
                    lineNum = block.jumpToIndex - 1;
                }
            }
            break;
        }
        case BlockType::ELSE:{
            if (block.jumpToIndex != - 1)
                lineNum = block.jumpToIndex -1;
            break;
        }

        case BlockType::REPEAT:{
            if(loops.find(lineNum) == loops.end()){
                int count = block.parameters.empty() ? 10 : (int)block.parameters[0];
                loops[lineNum] = count;
            }
            break;
        }

        case BlockType::END_REPEAT :{
            int startLine = block.jumpToIndex;
            if(loops.find(startLine) != loops.end()){
                loops[startLine] -- ;
                if(loops[startLine] > 0){
                    lineNum = startLine;
                } else {
                    loops.erase(startLine);
                }
            } else {
                lineNum = startLine;
            }
            break;
        }

        default:
            break;
    }
}
std::string getBlockName(BlockType type){
    switch (type) {
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

void runScript( std::vector<Block> &blocks , Sprite &sprite ){
    preprocessScript(blocks);
    static int cc =0 ; //cycleCounter
    cc++;

    map<int , int > loopCounters;

    for(int i=0 ; i<blocks.size() ; ++i){
        const Block& b = blocks[i];

        double x1 = sprite.x ;
        double y1 = sprite.y;
        double dir1 = sprite.direction;
        bool Vis1 = sprite.isVisible;

        executeBlock(b , sprite , i , loopCounters );

        int logline = i +1 ;
        std::string log_cmd = getBlockName(b.type);
        std::string log_detail = "Executed";

        if (std::abs(sprite.x - x1) > 0.001 || std::abs(sprite.y - y1) > 0.001){
            log_detail = "Pos: (" +std::to_string(int(x1))+","+std::to_string(int(y1))+") -> ("+
                    std::to_string(int(sprite.x))+","+std::to_string(int(sprite.y))+")";
        } else if (std::abs(sprite.direction - dir1)>0.001){
            log_detail = "Dir: (" +std::to_string(int(dir1))+" -> " +std::to_string(int(sprite.direction))+")";
        } else if(sprite.isVisible != Vis1){
            log_detail = (sprite.isVisible ? "Hidden -> Visible" : "Visible -> Hidden");
        }

        //inja bayad tabe log javad bashe.
    }
}
