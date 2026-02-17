//
// Created by jack on 2/17/2026.
//

#pragma once
#include "block.h"
#include "font.h"
#include <vector>

enum struct BlockCategory {
    MOTION,
    LOOKS,
    EVENTS,
    CONTROL
};

struct BlockPalette {
    SDL_Rect category_menu_rect;
    SDL_Rect block_panel_rect;

    BlockCategory selected_category = BlockCategory::MOTION;


    std::vector<Block> template_blocks;
};

void initBlockPalette(BlockPalette* palette);
void drawBlockPalette(SDL_Renderer* renderer, BlockPalette* palette, TTF_Font* font);