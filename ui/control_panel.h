//
// Created by jack on 2/18/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct ControlPanel {
    SDL_Rect rect;
    SDL_Rect green_flag_rect;
    SDL_Rect stop_button_rect;
    SDL_Texture* green_flag_texture;
    SDL_Texture* stop_button_texture;
};

void initControlPanel(ControlPanel* panel, SDL_Renderer* renderer);
void drawControlPanel(SDL_Renderer* renderer, const ControlPanel* panel);