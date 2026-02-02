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
    ELSE,
};

struct Block {
    BlockType type;
    std::vector<double> parameters;
    std::string textParam;
    int jumpToIndex = -1;
};

#endif //SCRATCHCLONE_SDL_BLOCK_H
