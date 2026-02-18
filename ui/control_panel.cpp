//
// Created by jack on 2/18/2026.
//
// ui/control_panel.cpp

#include <iostream>
#include "control_panel.h"
#include "../core/config.h"

void initControlPanel(ControlPanel* panel, SDL_Renderer* renderer) {

    panel->rect = {int(windowConfig.width * 0.65), 0, int(windowConfig.width * 0.35), int(windowConfig.height * 0.04)};

    int btn_size = panel->rect.h - 10;
    panel->green_flag_rect = {panel->rect.x + 10, 5, btn_size, btn_size};
    panel->stop_button_rect = {panel->rect.x + 10 + btn_size + 10, 5, btn_size, btn_size};

    SDL_Surface* flag_surface = IMG_Load((ASSETS_PATH + "icons/green_flag.bmp").c_str());
    if (flag_surface) {
        panel->green_flag_texture = SDL_CreateTextureFromSurface(renderer, flag_surface);
        SDL_FreeSurface(flag_surface);
    }

    SDL_Surface* stop_surface = IMG_Load((ASSETS_PATH + "icons/stop_sign.bmp").c_str());
    if (stop_surface) {
        panel->stop_button_texture = SDL_CreateTextureFromSurface(renderer, stop_surface);
        SDL_FreeSurface(stop_surface);
    }
}

void drawControlPanel(SDL_Renderer* renderer, const ControlPanel* panel) {
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &panel->rect);

    if (panel->green_flag_texture) {
        SDL_RenderCopy(renderer, panel->green_flag_texture, NULL, &panel->green_flag_rect);
    }
    if (panel->stop_button_texture) {
        SDL_RenderCopy(renderer, panel->stop_button_texture, NULL, &panel->stop_button_rect);
    }
}