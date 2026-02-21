//
// Created by jack on 2/17/2026.
//


#include "block_palette.h"
#include "../core/config.h"
#include "script_area.h"


Block createTemplate(BlockType type, std::vector<double> params = {}, std::string text = "") {
    return {type, params, text};
}

void initBlockPalette(BlockPalette* palette) {

    palette->category_menu_rect = {0, int(windowConfig.height * 0.04), 60, int(windowConfig.height * 0.96)};
    palette->block_panel_rect = {60, int(windowConfig.height * 0.04), 240, int(windowConfig.height * 0.96)};

    // motion
    palette->template_blocks.push_back(createTemplate(BlockType::MOVE, {10}));
    palette->template_blocks.push_back(createTemplate(BlockType::TURN_RIGHT, {15}));
    palette->template_blocks.push_back(createTemplate(BlockType::TURN_LEFT, {15}));
    palette->template_blocks.push_back(createTemplate(BlockType::GO_TO_XY, {0, 0}));
    // looks
    palette->template_blocks.push_back(createTemplate(BlockType::SAY, {}, "Hello!"));
    palette->template_blocks.push_back(createTemplate(BlockType::SHOW));
    palette->template_blocks.push_back(createTemplate(BlockType::HIDE));
    // events
    palette->template_blocks.push_back(createTemplate(BlockType::ON_FLAG_CLICKED));
    // control
    palette->template_blocks.push_back(createTemplate(BlockType::WAIT, {1}));
    palette->template_blocks.push_back(createTemplate(BlockType::REPEAT, {10}));
    palette->template_blocks.push_back(createTemplate(BlockType::IF));

}


void drawBlockPalette(SDL_Renderer* renderer, BlockPalette* palette, TTF_Font* font) {

    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &palette->category_menu_rect);


    SDL_Rect category_btn = {15, 60, 30, 30};
    SDL_SetRenderDrawColor(renderer, 76, 151, 255, 255); // آبی (Motion)
    SDL_RenderFillRect(renderer, &category_btn);

    category_btn.y += 40;
    SDL_SetRenderDrawColor(renderer, 156, 89, 209, 255); // بنفش (Looks)
    SDL_RenderFillRect(renderer, &category_btn);


    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    SDL_RenderFillRect(renderer, &palette->block_panel_rect);



    int current_y = palette->block_panel_rect.y + 20;
    for (auto& block_template : palette->template_blocks) {
        block_template.rect = {
                palette->block_panel_rect.x + 10,
                current_y,
                220,
                40
        };

        drawBlock(renderer, &block_template, font);
        current_y += 50;
    }
}