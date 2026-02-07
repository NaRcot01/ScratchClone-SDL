//
// Created by jack on 2/5/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include "sprite.h"
#include "font.h"

struct PropertyRow{
    std::string label;
    std::string value;
    SDL_Rect inputRect;
    bool active;
};

struct PropertyPanel{
    SDL_Rect rect;
    PropertyRow rows[5];
    bool visible;
};



void initPropertyPanel(PropertyPanel* panel, int screenWidth, int screenHeight);
void drawPropertyPanel(SDL_Renderer *renderer, PropertyPanel *panel, Sprite *activeSprite, TTF_Font *font);
bool handlePropertyPanelClicked(PropertyPanel* panel,Sprite* activeSprite, int m_x,int m_y);