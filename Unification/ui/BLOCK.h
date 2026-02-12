//
// Created by َAmir on 1/30/2026.
//

#ifndef SCRATCHCLONE_SDL_BLOCK_H
#define SCRATCHCLONE_SDL_BLOCK_H

#include <vector>
#include <string>

enum struct BlockType {
    MOVE , TURN_RIGHT , TURN_LEFT , GO_TO_XY ,
    SAY , SHOW , HIDE ,
    WAIT , REPEAT , IF , FOREVER ,
    END_REPEAT , END_IF ,
    ON_FLAG_CLICKED , CHANGE_X , CHANGE_Y , SET_SIZE ,
    ELSE, SET_VAR , CHANGE_VAR , BROADCAST ,
    OP_ADD , OP_SUB , OP_MUL , OP_DIV , OP_EQUAL , OP_LESS , OP_GREATER , RAND
};

struct Block {
    BlockType type;
    std::vector<double> parameters;
    std::string textParam;
    int jumpToIndex = -1;
};

void preprocessScript(std::vector<Block> &v);

#endif //SCRATCHCLONE_SDL_BLOCK_H
