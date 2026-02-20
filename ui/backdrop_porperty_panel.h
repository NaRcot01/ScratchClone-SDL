//
// Created by jack on 2/19/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "font.h"

struct BackdropPropertyPanel {
    SDL_Rect rect;
    SDL_Rect name_input_rect;
    bool is_visible = false;
    bool is_editing_name = false;

    SDL_Rect delete_button_rect;
    SDL_Texture* delete_button_texture;
};

void initBackdropPropertyPanel(BackdropPropertyPanel* panel, SDL_Renderer* renderer);
void drawBackdropPropertyPanel(SDL_Renderer* renderer, BackdropPropertyPanel* panel, const std::string& name, TTF_Font* font);