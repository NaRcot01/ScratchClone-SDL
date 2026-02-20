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
        {BlockType::IF,          {"If <> then",            {255, 171, 25, 255}}},
        {BlockType::ELSE,        {"Else",                  {255, 171, 25, 255}}},
        {BlockType::END_IF,      {"End If",                {255, 171, 25, 255}}},
        {BlockType::END_REPEAT,  {"End Repeat",            {255, 171, 25, 255}}},
};


void initScriptArea(ScriptArea* area) {
    area->rect.x =300;
    area->rect.y = windowConfig.height * 0.04;
    area->rect.w = windowConfig.width * 0.35;
    area->rect.h = windowConfig.height * 0.95;
    area->scroll_offset_y = 0;
}


void drawBlock(SDL_Renderer* renderer, Block* block, TTF_Font* font) {

    SDL_Color color = block_styles.count(block->type) ? block_styles[block->type].color : SDL_Color{128, 128, 128, 255};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &block->rect);

    SDL_SetRenderDrawColor(renderer, color.r - 20, color.g - 20, color.b - 20, 255);
    SDL_RenderDrawRect(renderer, &block->rect);


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



        std::string param_value_str;
        if (!block->textParam.empty()) {
            param_value_str = block->textParam;
        } else if (param_idx < block->parameters.size()) {
            param_value_str = std::to_string((int)block->parameters[param_idx]);
        }

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

            SDL_RenderDrawRect(renderer, &param_r);
            param_r.x -= 1; param_r.y -= 1; param_r.w += 2; param_r.h += 2;
            SDL_RenderDrawRect(renderer, &param_r);
        }

        drawText(renderer, font, param_value_str, param_r.x + 5, param_r.y + 2, {0, 0, 0, 255});


        current_x += param_r.w;
        param_idx++;

        start_pos = end_pos + 2;
        end_pos = text_template.find("{}", start_pos);
    }
}

void drawScriptArea(SDL_Renderer* renderer, ScriptArea* area, Sprite* activeSprite, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
    SDL_RenderFillRect(renderer, &area->rect);

    if (!activeSprite) {
        return;
    }

    for (const auto& script : activeSprite->scripts) {
        for (const auto& block : script) {
            drawBlock(renderer, const_cast<Block*>(&block), font);
        }
    }
}