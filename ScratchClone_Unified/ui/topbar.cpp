//
// Created by jack on 2/5/2026.
//
#include "topbar.h"
#include "topbar_button.h"
#include "../core/config.h"

void initTopBar(TopBar* bar){
    bar->rect.x = 0;
    bar->rect.y=0;
    bar->rect.w = windowConfig.width;
    bar->rect.h = windowConfig.height * 0.04;
    bar->buttonCount = 0;

    int currentX = 10;
    int btnWidth = bar->rect.w/12;

    auto addBtn = [&](TopBarButtonType type, std::string text) {
	if (bar->buttonCount >= TOPBAR_MAX_BUTTONS) return;
	TopBarButton btn;
	btn.rect.x =currentX;
	btn.rect.y = 5;
	btn.rect.h = bar->rect.h - 10;
	btn.rect.w = btnWidth;
	btn.type = type;
	btn.text = text;
	bar->buttons[bar->buttonCount++] = btn;
	currentX += btnWidth + 10;
    };

    addBtn(BTN_SPRITE_PANEL, "Sprite Panel");
    addBtn(BTN_NEW_PROJECT, "New");
    addBtn(BTN_SAVE_PROJECT, "Save");
    addBtn(BTN_LOAD_PROJECT, "Load");
    addBtn(BTN_PLAY,"Play");
}


void drawTopBar(SDL_Renderer* renderer, TopBar* bar,TTF_Font* font){
    SDL_SetRenderDrawColor(renderer,200,200,200,255);
    SDL_RenderFillRect(renderer,&bar->rect);

    SDL_SetRenderDrawColor(renderer,50,50,50,255);
    SDL_RenderDrawRect(renderer,&bar->rect);

    for(int i=0;i<bar->buttonCount;i++){
        TopBarButton& btn = bar->buttons[i];

        SDL_SetRenderDrawColor(renderer,70,70,70,255);
        SDL_RenderDrawRect(renderer,&bar->rect);
        SDL_SetRenderDrawColor(renderer,170,170,170,255);
        SDL_RenderFillRect(renderer,&bar->rect);
        renderTopBarBtn(renderer,btn,font);

    }
}