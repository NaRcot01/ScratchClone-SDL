//
// Created by jack on 2/4/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "font.h"



enum TopBarButtonType{
    BTN_SPRITE_PANEL,
    BTN_STAGE_PANEL
};

struct TopBarButton{
    SDL_Rect rect;
    TopBarButtonType type;
    std::string text;

    bool isClicked(int m_x,int m_y){
        SDL_Point p = {m_x,m_y};
        return SDL_PointInRect(&p,&this->rect);
    }
};

void renderTopBarBtn(SDL_Renderer* renderer, TopBarButton& btn,TTF_Font* font);