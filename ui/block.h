//
// Created by jack on 2/17/2026.
//


#pragma once
#include <vector>
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <memory>

enum struct BlockType {
    MOVE , TURN_RIGHT , TURN_LEFT , GO_TO_XY ,
    SAY , SHOW , HIDE ,
    WAIT , REPEAT , IF , FOREVER ,
    END_REPEAT , END_IF ,
    ON_FLAG_CLICKED , CHANGE_X , CHANGE_Y , SET_SIZE ,
    ELSE, SET_VAR , CHANGE_VAR , BROADCAST ,
    OP_ADD , OP_SUB , OP_MUL , OP_DIV , OP_EQUAL , OP_LESS , OP_GREATER , RAND , GREATER_THAN, X_POSITION , Y_POSITION, PLAY_SOUND,
    GO_TO_RANDOM_POSITION,
    SET_X,
    SET_Y,
    CHANGE_SIZE,

};

enum struct BlockCategory {
    MOTION,
    LOOKS,
    SOUND,
    EVENTS,
    CONTROL,
    SENSING,
    OPERATORS,
    VARIABLES
};

enum struct ParamType {
    NUMERIC,
    STRING,
    BLOCK_REPORTER,
    BLOCK_BOOLEAN
};

struct BlockAppearance {
    std::string text;
    SDL_Color color;

    std::vector<ParamType> param_types;
};

extern std::map<BlockType, BlockAppearance> block_styles;

struct Block {
    BlockType type;
    BlockCategory category;
    std::vector<double> parameters;
    std::vector<std::unique_ptr<Block>> block_parameters;

    std::string textParam;
    int jumpToIndex = -1;

    SDL_Rect rect;

    std::vector<SDL_Rect> param_rects;

    Block() = default;


    Block(BlockType t, BlockCategory cat, const std::vector<double>& params = {}, const std::string& text = "")
            : type(t), category(cat), parameters(params), textParam(text) {}

    Block(const Block& other)
            : type(other.type),
              category(other.category),
              parameters(other.parameters),
              textParam(other.textParam),
              jumpToIndex(other.jumpToIndex),
              rect(other.rect),
              param_rects(other.param_rects)
    {
        for (const auto& ptr : other.block_parameters) {
            if (ptr) {
                block_parameters.push_back(std::make_unique<Block>(*ptr));
            } else {
                block_parameters.push_back(nullptr);
            }
        }
    }

    Block& operator=(const Block& other) {
        if (this == &other) return *this;

        type = other.type;
        category = other.category;
        parameters = other.parameters;
        textParam = other.textParam;
        jumpToIndex = other.jumpToIndex;
        rect = other.rect;
        param_rects = other.param_rects;

        block_parameters.clear();
        for(const auto& ptr : other.block_parameters) {
            if (ptr) block_parameters.push_back(std::make_unique<Block>(*ptr));
            else block_parameters.push_back(nullptr);
        }
        return *this;
    }

    Block(Block&& other) noexcept = default;

    Block& operator=(Block&& other) noexcept = default;
};