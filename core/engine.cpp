//
// Created by jack on 1/31/2026.
//

#include <iostream>
#include <vector>
#include "engine.h"
#include "config.h"
#include "../ui/sprite_panel.h"
#include "../ui/sprite.h"
#include "../ui/stage.h"
#include "../ui/topbar.h"



int panelSelectedIndex = -1;


SDL_Event event;
Stage stage;
std::vector<Sprite> sprites;
SpritePanel spritePanel;
Sprite* activeSprite = NULL;
bool showSpritePanel = false;
TopBar topBar;
TTF_Font* font;

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
            if(activeSprite) {
                activeSprite->dragging = false;
                activeSprite = NULL;
            }
        }
        if (event.type == SDL_MOUSEMOTION) {
            if(activeSprite && activeSprite->dragging) {
                int m_x = event.button.x;
                int m_y = event.button.y;
                std::cout << " dragging sprite " << std::endl;
                // log :
                moveSprite(activeSprite, m_x - activeSprite->diff_x_mouse, m_y - activeSprite->diff_y_mouse, &stage);
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            bool clickOnPanel = false;

            int m_x = event.button.x;
            int m_y = event.button.y;

            for(int i=0;i<sprites.size();i++) {
                Sprite& sprite = sprites[i];

                SDL_Rect itemRect = {
                        spritePanel.rect.x + 10,
                        spritePanel.rect.y+10+i*(PANEL_ITEM_HEIGHT + PANEL_ITEM_MARGIN),
                        spritePanel.rect.w - 20,
                        PANEL_ITEM_HEIGHT
                };

                if (m_x >= itemRect.x &&
                    m_x <= itemRect.x + itemRect.w &&
                    m_y >= itemRect.y &&
                    m_y <= itemRect.y + itemRect.h)
                {
                    panelSelectedIndex = i;
                    clickOnPanel = true;
                    activeSprite = &sprites[i];
                    activeSprite->selected = true;
                    activeSprite->dragging = false;
                    for(int j =0 ;j< sprites.size();j++){
                        if(i!=j){
                            sprites[j].selected = false;
                        }
                    }
                    break;
                }
            }
            if(!clickOnPanel){
                activeSprite = NULL;
                panelSelectedIndex = -1;
                for(int i = 0; i<sprites.size();i++){
                    Sprite& sprite = sprites[i];
                    sprite.selected = false;
                    sprite.dragging = false;
                    if (isSpriteClicked(m_x, m_y, &sprite)) {
                        sprite.selected = true;
                        sprite.dragging = true;
                        std::cout << " sprite clicked " << std::endl;
                        sprite.diff_x_mouse = m_x - sprite.rect.x;
                        sprite.diff_y_mouse = m_y - sprite.rect.y;
                        activeSprite = &sprite;
                        panelSelectedIndex = i;
                        break;
                    }
                }
            }
            for(int i=0;i<topBar.buttonCount;i++){
                TopBarButton& btn = topBar.buttons[i];
                if(btn.isClicked(m_x,m_y)){
                    if(btn.type == BTN_SPRITE_PANEL){
                        showSpritePanel = true;
                    }
                }
            }

        }


    }
}

void engineUpdate() {

}


void initSprites(){
    sprites.resize(3); // should be removed.    JUST FOR TEST
    for(int i=0;i<sprites.size();i++){
        initSprite(sprites[i],&stage);
        sprites[i].rect.x += 50 * i;
    }
}

void drawSprites(SDL_Renderer *renderer){

    for(int i =0;i< sprites.size();i++){
        Sprite& sprite = sprites[i];
        drawSprite(renderer, &sprite);
    }
}

void drawSpritePanels(SDL_Renderer* renderer){
    drawSpritePanelBase(renderer,&spritePanel);
    for(int i =0;i< sprites.size();i++) {
        Sprite& sprite = sprites[i];
        drawSpritePanel(renderer, &spritePanel, &sprite,i,panelSelectedIndex);
    }
}

//this function will be called in the main file before the while loop
void initBase(SDL_Renderer *renderer) {
    font = loadFont();
    std::cout << font;
    initStage(&stage);
    initSprites();
    initSpritePanel(&spritePanel);
    initTopBar(&topBar);
    for(auto &sprite : sprites){
        loadSpriteTexture(renderer,sprite,ASSETS_PATH + "test.bmp");
    }
}

void engineDraw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderClear(renderer);

    drawStage(renderer, &stage);
    drawSprites(renderer);
    drawTopBar(renderer,&topBar,font);
    if(showSpritePanel){
        drawSpritePanels(renderer);
    }
//    drawTopBar(renderer,&topBar,font);

    SDL_RenderPresent(renderer);
}