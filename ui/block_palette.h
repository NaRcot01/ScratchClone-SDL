//
// Created by jack on 2/17/2026.
//

#pragma once
#include "block.h"
#include "font.h"
#include <vector>

struct CategoryButton {
    BlockCategory category;
    SDL_Rect rect;
    std::string text;
    SDL_Color color;
};

struct BlockPalette {
    SDL_Rect category_menu_rect;
    SDL_Rect block_panel_rect;

    BlockCategory selected_category = BlockCategory::MOTION;
    std::vector<CategoryButton> category_buttons;
    int scroll_offset_y = 0;


    std::vector<Block> template_blocks;
    SDL_Rect make_variable_button_rect;
    std::vector<Block> variable_blocks;
};

void initBlockPalette(BlockPalette* palette);
void drawBlockPalette(SDL_Renderer* renderer, BlockPalette* palette, TTF_Font* font);
Block createTemplate(BlockType type, BlockCategory cat, const std::vector<double> params, std::string text);