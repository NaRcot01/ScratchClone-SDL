//
// Created by jack on 2/19/2026.
//


#include "background_panel.h"
#include "../core/config.h"

void initBackgroundPanel(BackgroundPanel* panel, SDL_Renderer* renderer) {
    panel->rect.x = windowConfig.width * 0.325;
    panel->rect.y = windowConfig.height * 0.04;
    panel->rect.w = windowConfig.width * 0.2;
    panel->rect.h = windowConfig.height * 0.96;


    panel->upload_button_rect = {panel->rect.x + 10, panel->rect.y + 10, 40, 40};

    SDL_Surface* upload_surface = SDL_LoadBMP((ASSETS_PATH + "icons/upload.bmp").c_str());
    if (upload_surface) {
        panel->upload_button_texture = SDL_CreateTextureFromSurface(renderer, upload_surface);
        SDL_FreeSurface(upload_surface);
    }
}

void drawBackgroundPanel(SDL_Renderer* renderer, const BackgroundPanel* panel, Stage* stage,TTF_Font* font ) {
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderFillRect(renderer, &panel->rect);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &panel->rect);

    if (panel->upload_button_texture) {
        SDL_RenderCopy(renderer, panel->upload_button_texture, NULL, &panel->upload_button_rect);
    }

    int current_y = panel->upload_button_rect.y + panel->upload_button_rect.h + 10;
    for (int i = 0; i < stage->backgrounds.size(); ++i) {
        SDL_Rect item_rect = {
                panel->rect.x + 10,
                current_y,
                panel->rect.w - 20,
                100
        };

        if (i == stage->active_background_index) {
            SDL_SetRenderDrawColor(renderer, 76, 151, 255, 255);
            for(int j = 0; j < 3; ++j) {
                SDL_Rect border = {item_rect.x - j, item_rect.y - j, item_rect.w + j*2, item_rect.h + j*2};
                SDL_RenderDrawRect(renderer, &border);
            }
        }

        SDL_RenderCopy(renderer, stage->backgrounds[i].texture, NULL, &item_rect);
        drawText(renderer, font, stage->backgrounds[i].name, item_rect.x + 5, item_rect.y + item_rect.h - 20, {0,0,0,255});
        current_y += item_rect.h + 10;
    }
}