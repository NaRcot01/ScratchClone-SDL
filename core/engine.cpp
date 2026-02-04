//
// Created by jack on 1/31/2026.
//

#include <iostream>
#include "engine.h"
#include "config.h"
#include "../ui/sprite_panel.h"
#include "../ui/sprite.h"
#include "../ui/stage.h"

SDL_Event event;


Stage stage;
Sprite sprite;
SpritePanel spritePanel;

void engineInit(Engine &engine) {
    engine.running = true;
}

void engineHandleEvents(Engine &engine) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            engine.running = false;
            // log :
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            int W = event.window.data1;
            int H = event.window.data2;

            windowConfig.height = H;
            windowConfig.width = W;
            // log :
            resizeStage(&stage);
        }
        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
            sprite.dragging = false;
        }
        if (event.type == SDL_MOUSEMOTION && sprite.dragging) {
            int m_x = event.button.x;
            int m_y = event.button.y;
            std::cout << " dragging sprite " << std::endl;
            // log :
            moveSprite(&sprite, m_x - sprite.diff_x_mouse, m_y - sprite.diff_y_mouse, &stage);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int m_x = event.button.x;
            int m_y = event.button.y;

            if (isSpriteClicked(m_x, m_y, &sprite)) {
                sprite.selected = true;
                sprite.dragging = true;
                std::cout << " sprite clicked " << std::endl;
                sprite.diff_x_mouse = m_x - sprite.rect.x;
                sprite.diff_y_mouse = m_y - sprite.rect.y;
            } else {
                sprite.selected = false;
                sprite.dragging = false;
            }

            SDL_Rect itemRect = {
                    spritePanel.rect.x + 10,
                    20,
                    spritePanel.rect.w - 20,
                    60
            };

            if (m_x >= itemRect.x &&
                m_x <= itemRect.x + itemRect.w &&
                m_y >= itemRect.y &&
                m_y <= itemRect.y + itemRect.h)
            {
                sprite.selected = true;
            }
        }


    }
}

void engineUpdate() {

}

//this function will be called in the main file before the while loop
void initBase() {
    initStage(&stage);
    initSprite(sprite, &stage);
    initSpritePanel(&spritePanel);
}

void engineDraw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderClear(renderer);

    drawStage(renderer, &stage);
    drawSprite(renderer, &sprite);
    drawSpritePanel(renderer, &spritePanel, &sprite);

    SDL_RenderPresent(renderer);
}