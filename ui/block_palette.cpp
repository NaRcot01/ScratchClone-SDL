//
// Created by jack on 2/17/2026.
//


#include "block_palette.h"
#include "../core/config.h"
#include "script_area.h"


Block createTemplate(BlockType type, BlockCategory cat, const std::vector<double> params = {}, std::string text = "") {
    return {type, cat, params, text};
}

void initBlockPalette(BlockPalette* palette) {

    palette->category_menu_rect = {0, int(windowConfig.height * 0.04), 60, int(windowConfig.height * 0.96)};
    palette->block_panel_rect = {60, int(windowConfig.height * 0.04), 240, int(windowConfig.height * 0.96)};
    palette->selected_category = BlockCategory::MOTION;


    palette->category_buttons.clear();
    int btn_y = palette->category_menu_rect.y + 20;
    palette->category_buttons.push_back({BlockCategory::MOTION, {10, btn_y, 60, 25}, "Motion", {76, 151, 255, 255}});
    btn_y += 35;
    palette->category_buttons.push_back({BlockCategory::LOOKS, {10, btn_y, 60, 25}, "Looks", {156, 89, 209, 255}});
    btn_y += 35;
    palette->category_buttons.push_back({BlockCategory::EVENTS, {10, btn_y, 60, 25}, "Events", {255, 193, 7, 255}});
    btn_y += 35;
    palette->category_buttons.push_back({BlockCategory::CONTROL, {10, btn_y, 60, 25}, "Control", {255, 171, 25, 255}});
    btn_y += 35;
    palette->category_buttons.push_back({BlockCategory::OPERATORS, {10, btn_y, 60, 25}, "Operators", {83, 193, 83, 255}});


    // motion
    palette->template_blocks.push_back(createTemplate(BlockType::MOVE,BlockCategory::MOTION, {10}));
    palette->template_blocks.push_back(createTemplate(BlockType::TURN_RIGHT,BlockCategory::MOTION, {15}));
    palette->template_blocks.push_back(createTemplate(BlockType::TURN_LEFT,BlockCategory::MOTION, {15}));
    palette->template_blocks.push_back(createTemplate(BlockType::GO_TO_XY,BlockCategory::MOTION, {0, 0}));
    palette->template_blocks.push_back(createTemplate(BlockType::X_POSITION, BlockCategory::MOTION));
    palette->template_blocks.push_back(createTemplate(BlockType::Y_POSITION, BlockCategory::MOTION));
    // looks
    palette->template_blocks.push_back(createTemplate(BlockType::SAY,BlockCategory::LOOKS, {}, "Hello!"));
    palette->template_blocks.push_back(createTemplate(BlockType::SHOW,BlockCategory::LOOKS));
    palette->template_blocks.push_back(createTemplate(BlockType::HIDE,BlockCategory::LOOKS));
    // events
    palette->template_blocks.push_back(createTemplate(BlockType::ON_FLAG_CLICKED,BlockCategory::EVENTS));
    // control
    palette->template_blocks.push_back(createTemplate(BlockType::WAIT,BlockCategory::CONTROL, {1}));
    palette->template_blocks.push_back(createTemplate(BlockType::REPEAT,BlockCategory::CONTROL, {10}));
    palette->template_blocks.push_back(createTemplate(BlockType::IF,BlockCategory::CONTROL));
    // operators
    palette->template_blocks.push_back(createTemplate(BlockType::GREATER_THAN, BlockCategory::OPERATORS, {50, 0}));
}


void drawBlockPalette(SDL_Renderer* renderer, BlockPalette* palette, TTF_Font* font) {

    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &palette->category_menu_rect);

    for (const auto& btn : palette->category_buttons) {

        SDL_SetRenderDrawColor(renderer, btn.color.r, btn.color.g, btn.color.b, 255);
        SDL_RenderFillRect(renderer, &btn.rect);

        if (btn.category == palette->selected_category) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            for(int i = 0; i < 2; ++i) {
                SDL_Rect border = {btn.rect.x - i, btn.rect.y - i, btn.rect.w + i * 2, btn.rect.h + i * 2};
                SDL_RenderDrawRect(renderer, &border);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    SDL_RenderFillRect(renderer, &palette->block_panel_rect);

    int current_y = palette->block_panel_rect.y + 20 - palette->scroll_offset_y;

    for (auto& block_template : palette->template_blocks) {
        if (block_template.category == palette->selected_category) {

            block_template.rect = {
                    palette->block_panel_rect.x + 10,
                    current_y,
                    palette->block_panel_rect.w - 20,
                    40
            };

            drawBlock(renderer, &block_template, font);

            current_y += 50;
        }
    }
}