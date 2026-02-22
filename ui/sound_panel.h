//
// Created by jack on 2/22/2026.
//


#pragma once
#include "sprite.h"
#include <SDL2/SDL.h>
#include "font.h"

struct SoundPanel {
    SDL_Rect rect;
    SDL_Rect upload_button_rect;
    SDL_Texture* upload_button_texture;
};

void initSoundPanel(SoundPanel* panel, SDL_Renderer* renderer);
void drawSoundPanel(SDL_Renderer* renderer, SoundPanel* panel, Sprite* activeSprite, TTF_Font* font);