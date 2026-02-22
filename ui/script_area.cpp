//
// Created by jack on 2/17/2026.
//

#include "script_area.h"
#include "../core/config.h"
#include "font.h"


extern Block* active_editing_block;
extern int active_editing_param_index;

std::map<BlockType, BlockAppearance> block_styles = {
        // Motion Blocks (Blue)
        {BlockType::MOVE,        {"Move {} steps",         { 76, 151, 255, 255},{ParamType::NUMERIC}}},
        {BlockType::TURN_RIGHT,  {"Turn right {} degrees", { 76, 151, 255, 255},{ParamType::NUMERIC}}},
        {BlockType::TURN_LEFT,   {"Turn left {} degrees",  { 76, 151, 255, 255},{ParamType::NUMERIC}}},
        {BlockType::GO_TO_XY,    {"Go to x: {} y: {}",     { 76, 151, 255, 255},{ParamType::NUMERIC,ParamType::NUMERIC}}},
        {BlockType::X_POSITION,  {"x position",            { 76, 151, 255, 255}, {}}},
        {BlockType::Y_POSITION,  {"y position",            { 76, 151, 255, 255}, {}}},
        {BlockType::GO_TO_RANDOM_POSITION, {"Go to random position", {76, 151, 255, 255}, {}}},
        {BlockType::SET_X,         {"Set x to {}",          {76, 151, 255, 255}, {ParamType::NUMERIC}}},
        {BlockType::SET_Y,         {"Set y to {}",          {76, 151, 255, 255}, {ParamType::NUMERIC}}},

        // Looks Blocks (Purple)
        {BlockType::SAY,         {"Say {}",                {156, 89, 209, 255},{ParamType::STRING}}},
        {BlockType::SHOW,        {"Show",                  {156, 89, 209, 255}}},
        {BlockType::HIDE,        {"Hide",                  {156, 89, 209, 255}}},
        {BlockType::SET_SIZE,    {"Set size to {} %",      {156, 89, 209, 255},{ParamType::NUMERIC}}},

        // Events Blocks (Yellow)
        {BlockType::ON_FLAG_CLICKED, {"When flag clicked", {255, 193, 7, 255}}},

        // Control Blocks (Orange)
        {BlockType::WAIT,        {"Wait {} seconds",       {255, 171, 25, 255},{ParamType::NUMERIC}}},
        {BlockType::REPEAT,      {"Repeat {}",             {255, 171, 25, 255},{ParamType::NUMERIC}}},
        {BlockType::FOREVER,     {"Forever",               {255, 171, 25, 255}}},
        {BlockType::IF, {"If {} then", {255, 171, 25, 255}, {ParamType::BLOCK_BOOLEAN}}},
        {BlockType::ELSE,        {"Else",                  {255, 171, 25, 255}}},
        {BlockType::END_IF,      {"",                {255, 171, 25, 255}}},
        {BlockType::END_REPEAT,  {"",            {255, 171, 25, 255}}},

        // Operators (Green)
        {BlockType::GREATER_THAN, { "{} > {}", {83, 193, 83, 255}, {ParamType::NUMERIC, ParamType::NUMERIC}}},
        {BlockType::RAND,         { "{}", {255, 140, 25, 255}, {ParamType::STRING}}},

        // Sound Blocks (Pink)
        {BlockType::PLAY_SOUND, {"play sound {}", {208, 3, 147, 255}, {ParamType::STRING}}},

};


void initScriptArea(ScriptArea* area) {
    area->rect.x =300;
    area->rect.y = windowConfig.height * 0.04;
    area->rect.w = windowConfig.width * 0.35;
    area->rect.h = windowConfig.height * 0.95;
    area->scroll_offset_y = 0;
}

void drawHexagon(SDL_Renderer* renderer, const SDL_Rect& rect) {
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void drawRoundedRect(SDL_Renderer* renderer, const SDL_Rect& rect) {
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void drawBlock(SDL_Renderer* renderer, Block* block, TTF_Font* font) {
    BlockAppearance style = block_styles.count(block->type) ? block_styles[block->type] : BlockAppearance{"Unknown", {128, 128, 128, 255}, {}};
    SDL_Color color = block_styles.count(block->type) ? block_styles[block->type].color : SDL_Color{128, 128, 128, 255};
    if (block->type == BlockType::REPEAT || block->type == BlockType::FOREVER || block->type == BlockType::IF || block->type == BlockType::ELSE) {
        SDL_Rect top_part = {block->rect.x, block->rect.y, block->rect.w, 40};
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderFillRect(renderer, &top_part);

        SDL_Rect bottom_part = {block->rect.x, block->rect.y + block->rect.h - 10, block->rect.w, 10};
        SDL_RenderFillRect(renderer, &bottom_part);


        SDL_Rect side_part = {block->rect.x, block->rect.y + 40, 20, block->rect.h - 50};
        SDL_RenderFillRect(renderer, &side_part);
    }
    else {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderFillRect(renderer, &block->rect);
    }


    block->param_rects.clear();


    std::string text_template = block_styles.count(block->type) ? block_styles[block->type].text : "Unknown Block";

    int current_x = block->rect.x + 10;
    int current_y = block->rect.y + 10;
    int param_idx = 0;

    size_t start_pos = 0;
    size_t end_pos = text_template.find("{}");

    while (start_pos < text_template.length()) {

        std::string text_part = text_template.substr(start_pos, end_pos - start_pos);
        if (!text_part.empty()) {
            drawText(renderer, font, text_part, current_x, current_y, {255, 255, 255, 255});
            int text_w, text_h;
            my_TTF_SizeUTF8(font, text_part.c_str(), &text_w, &text_h);
            current_x += text_w;
        }

        if (end_pos == std::string::npos) break;

        if (param_idx < style.param_types.size()) {
            ParamType p_type = style.param_types[param_idx];

            if (p_type == ParamType::BLOCK_BOOLEAN || p_type == ParamType::BLOCK_REPORTER) {

                if (param_idx < block->block_parameters.size() && block->block_parameters[param_idx]) {

                    Block inner_block_copy = *block->block_parameters[param_idx];

                    inner_block_copy.rect = {current_x, block->rect.y + 5, 80, 30};

                    drawBlock(renderer, &inner_block_copy, font);

                    current_x += inner_block_copy.rect.w + 5;
                }
                else {
                    SDL_Rect placeholder_rect = {current_x, block->rect.y + 5, 80, 30};
                    block->param_rects.push_back(placeholder_rect);

                    if (p_type == ParamType::BLOCK_BOOLEAN) drawHexagon(renderer, placeholder_rect);
                    else drawRoundedRect(renderer, placeholder_rect);

                    if (active_editing_block == block && active_editing_param_index == param_idx) {
                        SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);
                        for(int i=0; i<2; ++i) {
                            SDL_Rect border = {placeholder_rect.x-i, placeholder_rect.y-i, placeholder_rect.w+i*2, placeholder_rect.h+i*2};
                            SDL_RenderDrawRect(renderer, &border);
                        }
                    }
                    current_x += placeholder_rect.w + 5;
                }
            }
            else {
                std::string param_value_str;
                if (p_type == ParamType::STRING) {
                    param_value_str = block->textParam;
                } else if (p_type == ParamType::NUMERIC && param_idx < block->parameters.size()) {
                    param_value_str = std::to_string((int)block->parameters[param_idx]);
                }
                if (param_value_str.empty()) param_value_str = " ";

                int input_w, input_h;
                my_TTF_SizeUTF8(font, param_value_str.c_str(), &input_w, &input_h);
                input_w += 10;
                if (input_w < 30) input_w = 30;

                SDL_Rect param_r = {current_x, current_y - 2, input_w, input_h + 4};
                block->param_rects.push_back(param_r);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &param_r);

                if (active_editing_block == block && active_editing_param_index == param_idx) {
                    SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);
                    for(int i=0; i<2; ++i) {
                        SDL_Rect border = {param_r.x-i, param_r.y-i, param_r.w+i*2, param_r.h+i*2};
                        SDL_RenderDrawRect(renderer, &border);
                    }
                }

                drawText(renderer, font, param_value_str, param_r.x + 5, param_r.y + 2, {0, 0, 0, 255});
                current_x += param_r.w + 5;
            }
        }

        param_idx++;
        start_pos = end_pos + 2;
        end_pos = text_template.find("{}", start_pos);
    }
}

void calculateAndDrawScript(SDL_Renderer* renderer, std::vector<Block>& script, int start_index, int end_index, int& current_x, int& current_y, TTF_Font* font) {
    for (int i = start_index; i < end_index; ) {
        Block& block = script[i];

        if (block.type == BlockType::END_REPEAT || block.type == BlockType::END_IF || block.type == BlockType::ELSE) {
            i++;
            continue;
        }

        block.rect = {current_x, current_y, 220, 40};


        if (block.type == BlockType::REPEAT || block.type == BlockType::FOREVER) {
            int inner_block_start_index = i + 1;
            int inner_end_index  = block.jumpToIndex;

            int inner_x = current_x + 20;
            int inner_y = current_y + block.rect.h;
            int inner_content_height = 0;
            if (inner_end_index  > inner_block_start_index) {
                int temp_y_start = inner_y;
                calculateAndDrawScript(renderer, script, inner_block_start_index, inner_end_index, inner_x, temp_y_start, font);
                inner_content_height = temp_y_start - inner_y;
            }
            if (inner_content_height == 0) {
                inner_content_height = 30;
            }

            block.rect.h = 40 + inner_content_height + 10;


            drawBlock(renderer, &block, font);

            current_y += block.rect.h + BLOCK_SPACING;

            i = inner_end_index  + 1;
        }
        else if (block.type == BlockType::IF) {
            int inner_if_start_index = i + 1;
            int end_if_or_else_index = block.jumpToIndex;

            int if_content_height = 0;
            if (end_if_or_else_index > inner_if_start_index) {
                int inner_x = current_x + 20;
                int temp_y = current_y + 40;
                calculateAndDrawScript(renderer, script, inner_if_start_index,end_if_or_else_index, inner_x, temp_y, font);
                if_content_height = temp_y - (current_y + 40);
            }
            if (if_content_height == 0) if_content_height = 30;

            block.rect.h = 40 + if_content_height + 10;
            drawBlock(renderer, &block, font);

            if (end_if_or_else_index < script.size() && script[end_if_or_else_index].type == BlockType::ELSE) {
                Block& else_block = script[end_if_or_else_index];
                int inner_else_start_index = end_if_or_else_index + 1;
                int end_if_index = else_block.jumpToIndex;

                else_block.rect = {current_x, current_y + block.rect.h, 220, 40};

                int else_content_height = 0;
                if (end_if_index > inner_else_start_index) {
                    int inner_x = current_x + 20;
                    int temp_y = else_block.rect.y + 40;
                    calculateAndDrawScript(renderer, script, inner_else_start_index,end_if_index, inner_x, temp_y, font);
                    else_content_height = temp_y - (else_block.rect.y + 40);
                }
                if (else_content_height == 0) else_content_height = 30;

                else_block.rect.h = 40 + else_content_height + 10;
                drawBlock(renderer, &else_block, font);

                current_y += block.rect.h + else_block.rect.h + BLOCK_SPACING;
                i = end_if_index + 1;
            }
            else {
                current_y += block.rect.h + BLOCK_SPACING;
                i = end_if_or_else_index + 1;
            }
        }
        else {
            drawBlock(renderer, &block, font);
            current_y += block.rect.h + BLOCK_SPACING;
            i++;
        }
    }
}

void drawScriptArea(SDL_Renderer* renderer, ScriptArea* area, Sprite* activeSprite, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
    SDL_RenderFillRect(renderer, &area->rect);

    if (!activeSprite) {
        return;
    }

    for (auto& script : activeSprite->scripts) {
        if (script.empty()) continue;

        int current_x = script.front().rect.x;
        int current_y = script.front().rect.y;

        calculateAndDrawScript(renderer, script, 0,script.size(), current_x, current_y, font);
    }
}

void calculateLayout(std::vector<Block>& script, int start_index, int end_index, int& current_x, int& current_y) {
    for (int i = start_index; i < end_index; ) {
        Block& block = script[i];

        block.rect = {current_x, current_y, 220, 40};

        if (block.type == BlockType::IF || block.type == BlockType::REPEAT || block.type == BlockType::FOREVER) {
            int end_block_or_else = block.jumpToIndex;
            if (end_block_or_else <= i) { i++; continue; }

            int content_height = 0;
            if (end_block_or_else > i + 1) {
                int inner_y = current_y + 40;
                int inner_x = current_x + 20;
                calculateLayout(script, i + 1, end_block_or_else, inner_x, inner_y);
                content_height = inner_y - (current_y + 40);
            }
            if (content_height == 0) content_height = 30;
            block.rect.h = 40 + content_height + 10;

            if (block.type == BlockType::IF && end_block_or_else < script.size() && script[end_block_or_else].type == BlockType::ELSE) {
                Block& else_block = script[end_block_or_else];
                int end_if = else_block.jumpToIndex;
                if(end_if <= end_block_or_else) { i = end_block_or_else + 1; continue; }

                else_block.rect = {current_x, current_y + block.rect.h, 220, 40};

                content_height = 0;
                if (end_if > end_block_or_else + 1) {
                    int inner_y = else_block.rect.y + 40;
                    int inner_x = current_x + 20;
                    calculateLayout(script, end_block_or_else + 1, end_if, inner_x, inner_y);
                    content_height = inner_y - (else_block.rect.y + 40);
                }
                if (content_height == 0) content_height = 30;
                else_block.rect.h = 40 + content_height + 10;

                current_y += block.rect.h + else_block.rect.h + BLOCK_SPACING;
                i = end_if + 1;
            } else {
                current_y += block.rect.h + BLOCK_SPACING;
                i = end_block_or_else + 1;
            }
        }
        else {
            current_y += block.rect.h + BLOCK_SPACING;
            i++;
        }
    }
}