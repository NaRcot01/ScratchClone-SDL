//
// Created by jack on 2/17/2026.
//


#pragma once
#include <vector>
#include <string>
#include <map>
#include <SDL2/SDL.h>

enum struct BlockType {
    MOVE , TURN_RIGHT , TURN_LEFT , GO_TO_XY ,
    SAY , SHOW , HIDE ,
    WAIT , REPEAT , IF , FOREVER ,
    END_REPEAT , END_IF ,
    ON_FLAG_CLICKED , CHANGE_X , CHANGE_Y , SET_SIZE ,
    ELSE, SET_VAR , CHANGE_VAR , BROADCAST ,
    OP_ADD , OP_SUB , OP_MUL , OP_DIV , OP_EQUAL , OP_LESS , OP_GREATER , RAND
};

enum struct ParamType {
    NUMERIC,
    STRING
};

struct BlockAppearance {
    std::string text;
    SDL_Color color;

    std::vector<ParamType> param_types;
};

extern std::map<BlockType, BlockAppearance> block_styles;

struct Block {
    BlockType type;
    std::vector<double> parameters;
    std::string textParam;
    int jumpToIndex = -1;

    SDL_Rect rect;

    std::vector<SDL_Rect> param_rects;
};