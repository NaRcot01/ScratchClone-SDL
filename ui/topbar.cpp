//
// Created by jack on 2/5/2026.
//
#include "topbar.h"
#include "topbar_button.h"
#include "../core/config.h"

void initTopBar(TopBar *bar) {
    bar->rect.x = 0;
    bar->rect.y = 0;
    bar->rect.w = windowConfig.width;
    bar->rect.h = windowConfig.height * 0.04;
    bar->buttonCount = 0;


    TopBarButton sprite_btn;
    sprite_btn.rect.x = 10;
    sprite_btn.rect.y = 5;
    sprite_btn.rect.h = bar->rect.h - 10;
    sprite_btn.rect.w = bar->rect.w / 20 + 20;
    sprite_btn.type = BTN_SPRITE_PANEL;
    sprite_btn.text = "Sprite Panel";

    bar->buttons[bar->buttonCount] = sprite_btn;
    bar->buttonCount++;

    TopBarButton sounds_btn;
    sounds_btn.rect.x = sprite_btn.rect.x + sprite_btn.rect.w + 10;
    sounds_btn.rect.y = 5;
    sounds_btn.rect.h = bar->rect.h - 10;
    sounds_btn.rect.w = bar->rect.w / 15;
    sounds_btn.type = BTN_SOUNDS_PANEL;
    sounds_btn.text = "Sounds";
    bar->buttons[bar->buttonCount] = sounds_btn;
    bar->buttonCount++;



    TopBarButton stage_btn;
    stage_btn.rect.x = sounds_btn.rect.x + sounds_btn.rect.w + 20;
    stage_btn.rect.y = 5;
    stage_btn.rect.h = bar->rect.h - 10;
    stage_btn.rect.w = bar->rect.w / 15;
    stage_btn.type = BTN_STAGE_PANEL;
    stage_btn.text = "Stage Panel";

    bar->buttons[bar->buttonCount] = stage_btn;
    bar->buttonCount++;



    TopBarButton new_btn;
    new_btn.rect.x = stage_btn.rect.x + stage_btn.rect.w + 30;
    new_btn.rect.y = 5;
    new_btn.rect.h = bar->rect.h - 10;
    new_btn.rect.w = bar->rect.w / 20;
    new_btn.type = BTN_NEW_PROJECT;
    new_btn.text = "New";
    bar->buttons[bar->buttonCount] = new_btn;
    bar->buttonCount++;

    TopBarButton save_btn;
    save_btn.rect.x = new_btn.rect.x + new_btn.rect.w + 30;
    save_btn.rect.y = 5;
    save_btn.rect.h = bar->rect.h - 10;
    save_btn.rect.w = bar->rect.w / 20;
    save_btn.type = BTN_SAVE_PROJECT;
    save_btn.text = "Save";
    bar->buttons[bar->buttonCount] = save_btn;
    bar->buttonCount++;

    TopBarButton load_btn;
    load_btn.rect.x = save_btn.rect.x + save_btn.rect.w + 10;
    load_btn.rect.y = 5;
    load_btn.rect.h = bar->rect.h - 10;
    load_btn.rect.w = bar->rect.w / 20;
    load_btn.type = BTN_LOAD_PROJECT;
    load_btn.text = "Load";
    bar->buttons[bar->buttonCount] = load_btn;
    bar->buttonCount++;


}


void drawTopBar(SDL_Renderer *renderer, TopBar *bar, TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &bar->rect);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderDrawRect(renderer, &bar->rect);

    for (int i = 0; i < bar->buttonCount; i++) {
        TopBarButton &btn = bar->buttons[i];

        SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
        SDL_RenderDrawRect(renderer, &btn.rect);
        SDL_SetRenderDrawColor(renderer, 170, 170, 170, 255);
        SDL_RenderFillRect(renderer, &btn.rect);
        renderTopBarBtn(renderer, btn, font);

    }
}