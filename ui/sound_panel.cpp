//
// Created by jack on 2/22/2026.
//


#include "sound_panel.h"
#include "../core/config.h"
#include "font.h"

void initSoundPanel(SoundPanel* panel, SDL_Renderer* renderer) {
    panel->rect = {0, int(windowConfig.height * 0.04), 300, int(windowConfig.height * 0.96)};

    panel->upload_button_rect = {panel->rect.x + 10, panel->rect.y + 60, 35, 35};

    SDL_Surface* upload_surface = SDL_LoadBMP((ASSETS_PATH + "icons/upload.bmp").c_str());
    if (upload_surface) {
        panel->upload_button_texture = SDL_CreateTextureFromSurface(renderer, upload_surface);
        SDL_FreeSurface(upload_surface);
    }
}

void drawSoundPanel(SDL_Renderer* renderer, SoundPanel* panel, Sprite* activeSprite, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    SDL_RenderFillRect(renderer, &panel->rect);
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderDrawRect(renderer, &panel->rect);

    if (!activeSprite) return;

    drawText(renderer, font, "Sounds Panel", panel->rect.x + 20, panel->rect.y + 20, {0,0,0,255});

    if (panel->upload_button_texture) {
        SDL_RenderCopy(renderer, panel->upload_button_texture, NULL, &panel->upload_button_rect);
    }

    if (!activeSprite) return;

    int current_y = panel->upload_button_rect.y + panel->upload_button_rect.h + 20;
    for (const auto& sound : activeSprite->sounds) {
        SDL_Rect item_rect = {panel->rect.x + 10, current_y, panel->rect.w - 20, 50};

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &item_rect);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderDrawRect(renderer, &item_rect);

        // TODO: آیکون Play

        drawText(renderer, font, sound.name, item_rect.x + 40, item_rect.y + 15, {0,0,0,255});

        current_y += item_rect.h + 10;
    }
}