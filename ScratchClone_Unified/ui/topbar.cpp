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
    
    std::string texts[] = {"Sprite Panel", "New", "Save", "Load", "Play", "Pause", "Stop"};
    TopBarButtonType types[] = {
	BTN_SPRITE_PANEL, BTN_NEW_PROJECT, BTN_SAVE_PROJECT,
	BTN_LOAD_PROJECT, BTN_PLAY, BTN_PAUSE, BTN_STOP
	};

	for(int i = 0; i < 7; i++) {
		if (bar->buttonCount >= TOPBAR_MAX_BUTTONS) break;
	
		TopBarButton btn;
		btn.rect.x = currentX;
		btn.rect.y = 5;
		btn.rect.h = bar->rect.h-10;

		if (i==10) {
			btn.rect.w = 120;
			}
		else {
			btn.rect.w = 70;
			}

		btn.type = types[i];
		btn.text = texts[i];

		bar->buttons[bar->buttonCount] = btn;
		bar->buttonCount++;
		
		currentX += btn.rect.w + 10;
	}
}


void drawTopBar(SDL_Renderer* renderer, TopBar* bar,TTF_Font* font){
    SDL_SetRenderDrawColor(renderer,200,200,200,255);
    SDL_RenderFillRect(renderer,&bar->rect);

    SDL_SetRenderDrawColor(renderer,50,50,50,255);
    SDL_RenderDrawRect(renderer,&bar->rect);

    for(int i=0;i<bar->buttonCount;i++){
        TopBarButton& btn = bar->buttons[i];

        SDL_SetRenderDrawColor(renderer,70,70,70,255);
        SDL_RenderDrawRect(renderer,&btn.rect);
        SDL_SetRenderDrawColor(renderer,170,170,170,255);
        SDL_RenderFillRect(renderer,&btn.rect);
        renderTopBarBtn(renderer,btn,font);

    }
}