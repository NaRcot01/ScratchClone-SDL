//
// Created by jack on 2/5/2026.
//
#include "topbar_button.h"
#include "font.h"

void renderTopBarBtn(SDL_Renderer* renderer, TopBarButton& btn,TTF_Font* font){

    SDL_Texture* textTexture = renderText(renderer,font,btn.text,{255,255,255,255});

    int textWidth,textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr,&textWidth,&textHeight);
    SDL_Rect textRect = {
            btn.rect.x + (btn.rect.w - textWidth)/2,
            btn.rect.y + (btn.rect.h - textHeight)/2,
            textWidth,
            textHeight
    };

    SDL_RenderCopy(renderer,textTexture, nullptr,&textRect);
    SDL_DestroyTexture(textTexture);

}