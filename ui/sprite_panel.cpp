//
// Created by jack on 2/4/2026.
//
#include "sprite_panel.h"
#include "../core/config.h"


void initSpritePanel(SpritePanel *panel, SDL_Renderer *renderer) {
    panel->rect.x = windowConfig.width * 0.325;
    panel->rect.y = windowConfig.height * 0.04;
    panel->rect.w = windowConfig.width * 0.2;
    panel->rect.h = windowConfig.height * 0.96;


    std::string icon_files[4] = {
            "icons/upload.bmp",
            "icons/library.bmp",
            "icons/random.bmp",
            "icons/paint.bmp",
    };

    int buttonSize = panel->rect.w * 0.1;
    int totalButtonWidth = buttonSize * 4 + 30;
    int startX = panel->rect.x + (panel->rect.w - 200) / 2;
    int startY = panel->rect.y + 15;

    for (int i = 0; i < 4; i++) {
        panel->buttonRects[i] = {
                startX + i * (buttonSize + 30),
                startY,
                buttonSize,
                buttonSize,
        };
        std::string path = ASSETS_PATH + icon_files[i];
        SDL_Surface *surface = SDL_LoadBMP(path.c_str());
        if (!surface) {
            // log : error while loading icon_files[i]
            panel->buttonTextures[i] = nullptr;
            continue;
        }

        panel->buttonTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void drawSpritePanelBase(SDL_Renderer *renderer, const SpritePanel *panel) {
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderFillRect(renderer, &panel->rect);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &panel->rect);


    for (int i = 0; i < 4; i++) {
        if(panel->buttonTextures[i]){
            SDL_RenderCopy(renderer,panel->buttonTextures[i], nullptr,&panel->buttonRects[i]);
        }
    }
}

void drawSpritePanelItem(SDL_Renderer *renderer, const SpritePanel *panel, const Sprite *sprite, int index,
                         int panelSelectedIndex) {


    SDL_Rect item;
    item.w = panel->rect.w - 20;
    item.h = PANEL_ITEM_HEIGHT;
    item.x = panel->rect.x + 10;
    item.y = panel->rect.y + 50 + index * (PANEL_ITEM_MARGIN + PANEL_ITEM_HEIGHT) + 20;

    if (index == panelSelectedIndex) {
        SDL_SetRenderDrawColor(renderer, 255, 220, 150, 255);
        SDL_RenderFillRect(renderer, &item);
    }
    if (sprite->texture) {
        SDL_Rect icon = {
                item.x + 10,
                item.y + 10,
                40,
                40
        };
        SDL_RenderCopy(renderer, sprite->texture, NULL, &icon);
    } else {
        SDL_SetRenderDrawColor(renderer, 120, 170, 255, 255);
        SDL_Rect icon = {
                item.x + 10,
                item.y + 10,
                40, 40
        };
        SDL_RenderFillRect(renderer, &icon);
    }

    int deleteBtnSize = PANEL_ITEM_HEIGHT * 0.5;
    SDL_Rect deleteBtnRect = {
            item.x + item.w - deleteBtnSize - 5,
            item.y + (item.h - deleteBtnSize) / 2,
            deleteBtnSize,
            deleteBtnSize
    };

    SDL_Surface *surface = SDL_LoadBMP((ASSETS_PATH + "icons/delete.bmp").c_str());
    if (!surface) {
        // log : delete icon not found !
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer, texture, nullptr, &deleteBtnRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
