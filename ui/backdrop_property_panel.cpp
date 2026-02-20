//
// Created by jack on 2/19/2026.
//

#include "backdrop_porperty_panel.h"
#include "../core/config.h"
#include "font.h" // برای drawText

void initBackdropPropertyPanel(BackdropPropertyPanel* panel, SDL_Renderer* renderer) {

    float bg_panel_end_x = (windowConfig.width * 0.325f) + (windowConfig.width * 0.2f);
    panel->rect = {int(bg_panel_end_x), int(windowConfig.height * 0.04), 200, 200};

    panel->name_input_rect = {panel->rect.x + 10, panel->rect.y + 40, panel->rect.w - 20, 30};

    panel->delete_button_rect = {
            panel->name_input_rect.x,
            panel->name_input_rect.y + panel->name_input_rect.h + 10,
            30,
            30
    };
    SDL_Surface* delete_surface = SDL_LoadBMP((ASSETS_PATH + "icons/delete.bmp").c_str());
    if (delete_surface) {
        panel->delete_button_texture = SDL_CreateTextureFromSurface(renderer, delete_surface);
        SDL_FreeSurface(delete_surface);
    }
}

void drawBackdropPropertyPanel(SDL_Renderer* renderer, BackdropPropertyPanel* panel, const std::string& name, TTF_Font* font) {
    if (!panel->is_visible) {
        return;
    }


    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
    SDL_RenderFillRect(renderer, &panel->rect);
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderDrawRect(renderer, &panel->rect);


    drawText(renderer, font, "Backdrop Name:", panel->rect.x + 10, panel->rect.y + 10, {0, 0, 0, 255});


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &panel->name_input_rect);

    if (panel->is_editing_name) {
        SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);
        for(int i=0; i<2; ++i) {
            SDL_Rect border = {panel->name_input_rect.x-i, panel->name_input_rect.y-i, panel->name_input_rect.w+i*2, panel->name_input_rect.h+i*2};
            SDL_RenderDrawRect(renderer, &border);
        }
    }
    if (panel->delete_button_texture) {
        SDL_RenderCopy(renderer, panel->delete_button_texture, NULL, &panel->delete_button_rect);
    }

    drawText(renderer, font, name, panel->name_input_rect.x + 5, panel->name_input_rect.y + 5, {0, 0, 0, 255});
}