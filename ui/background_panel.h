//
// Created by jack on 2/19/2026.
//

#pragma once

#include "stage.h"
#include <SDL2/SDL.h>
#include "font.h"

struct BackgroundPanel {
    SDL_Rect rect;


    SDL_Rect upload_button_rect;
    SDL_Texture *upload_button_texture;
};

void initBackgroundPanel(BackgroundPanel *panel, SDL_Renderer *renderer);

void drawBackgroundPanel(SDL_Renderer *renderer, const BackgroundPanel *panel, Stage *stage, TTF_Font *font);