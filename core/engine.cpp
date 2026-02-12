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
#include "../ui/property_panel.h"


int panelSelectedIndex = -1;


SDL_Event event;
Stage stage;
std::vector<Sprite> sprites;
SpritePanel spritePanel;
Sprite *activeSprite = NULL;
bool showSpritePanel = false;
TopBar topBar;
TTF_Font *font;
PropertyPanel propertyPanel;


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
            if (activeSprite) {
                activeSprite->dragging = false;

            }
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE) {
            for (auto &row: propertyPanel.rows) {
                if (row.active && !row.value.empty()) {
                    row.value.pop_back();
                    applyPropertyToSprite(row, activeSprite, &stage);
                }
            }
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
            for (auto &row: propertyPanel.rows) {
                if (!row.active) { continue; }

                applyPropertyToSprite(row, activeSprite, &stage);
                spriteValidate(activeSprite, &stage);
                row.active = false;
            }
        } else if (event.type == SDL_KEYDOWN && propertyPanel.visible && activeSprite) {
            for (auto &row: propertyPanel.rows) {
                if (!row.active) { continue; }

                char c = event.key.keysym.sym;

                if (row.type != "PROPERTY_NAME") {
                    if (c >= SDLK_0 && c <= SDLK_9) {
                        row.value += char('0' + c - SDLK_0);
                    }
                } else {
                    if ((c >= SDLK_a && c <= SDLK_z) || (c >= SDLK_0 && c <= SDLK_9) || (c == SDLK_UNDERSCORE)) {
                        row.value += char('a' + c - SDLK_a);
                    }
                }
                applyPropertyToSprite(row, activeSprite, &stage);
            }
        }

        if (event.type == SDL_MOUSEMOTION) {
            if (activeSprite && activeSprite->dragging) {
                int m_x = event.button.x;
                int m_y = event.button.y;
                std::cout << " dragging sprite " << std::endl;
                // log :
                moveSprite(activeSprite, m_x - activeSprite->diff_x_mouse, m_y - activeSprite->diff_y_mouse, &stage);
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            if (activeSprite) {
                spriteValidate(activeSprite, &stage);  // it will check if the X,Y of the sprite is correct
            }
            bool clickOnPanel = false;

            int m_x = event.button.x;
            int m_y = event.button.y;
            SDL_Point p = {m_x, m_y};

            if (propertyPanel.visible && handlePropertyPanelClicked(&propertyPanel, activeSprite, m_x, m_y)) {
                return; // mouse is clicked on property panel so there is no need to check other conditions.
            }

            for (int i = 0; i < sprites.size(); i++) { // checks if mouse is clicked on sprite item in sprite panel.
                if (!showSpritePanel) { continue; }
                Sprite &sprite = sprites[i];

                SDL_Rect itemRect = {
                        spritePanel.rect.x + 10,
                        spritePanel.rect.y + 10 + i * (PANEL_ITEM_HEIGHT + PANEL_ITEM_MARGIN) + 20,
                        spritePanel.rect.w - 20,
                        PANEL_ITEM_HEIGHT
                };

                int deleteBtnSize = PANEL_ITEM_HEIGHT * 0.5;
                SDL_Rect deleteBtnRect = {
                        itemRect.x + itemRect.w - deleteBtnSize - 5,
                        itemRect.y + (itemRect.h - deleteBtnSize) / 2,
                        deleteBtnSize,
                        deleteBtnSize
                };

                if (SDL_PointInRect(&p, &deleteBtnRect)) {
                    if (activeSprite == &sprite) {
                        activeSprite = NULL;
                        panelSelectedIndex = -1;
                        propertyPanel.visible = false;
                    }

                    sprites.erase(sprites.begin() + i);
                    clickOnPanel = true;
                    break;
                }

                if (m_x >= itemRect.x &&
                    m_x <= itemRect.x + itemRect.w &&
                    m_y >= itemRect.y &&
                    m_y <= itemRect.y + itemRect.h) {
                    panelSelectedIndex = i;
                    clickOnPanel = true;
                    activeSprite = &sprites[i];
                    propertyPanel.visible = true;
                    activeSprite->selected = true;
                    activeSprite->dragging = false;
                    for (int j = 0; j < sprites.size(); j++) { //make sure other sprites are deselected
                        if (i != j) {
                            sprites[j].selected = false;
                        }
                    }
                    break;
                }
            }
            if (!clickOnPanel) { // checks if mouse is clicked on sprite on the stage.
                activeSprite = NULL;
                panelSelectedIndex = -1;
                propertyPanel.visible = false;

                for (int i = 0; i < sprites.size(); i++) {
                    Sprite &sprite = sprites[i];
                    if (!sprite.show) { continue; } // if the sprite is hidden, then it is unclickable on the stage!
                    sprite.selected = false;
                    sprite.dragging = false;
                    if (isSpriteClicked(m_x, m_y, &sprite)) {
                        sprite.selected = true;
                        sprite.dragging = true;
                        std::cout << " sprite clicked " << std::endl;
                        sprite.diff_x_mouse = m_x - sprite.rect.x;
                        sprite.diff_y_mouse = m_y - sprite.rect.y;
                        activeSprite = &sprite;
                        propertyPanel.visible = true;
                        panelSelectedIndex = i;
                        break;
                    }
                }
            }

            for (int i = 0; i < topBar.buttonCount; i++) {
                TopBarButton &btn = topBar.buttons[i];
                if (btn.isClicked(m_x, m_y)) {
                    if (btn.type == BTN_SPRITE_PANEL) {
                        showSpritePanel = !showSpritePanel;
                    }
                }
            }
        }
    }
}

void engineUpdate() {

}


void initSprites(SDL_Renderer *renderer) {
    sprites.resize(3); // should be removed.    JUST FOR TEST
    for (int i = 0; i < sprites.size(); i++) {
        initSprite(sprites[i], &stage);
        sprites[i].rect.x += 50 * i;
        loadSpriteTexture(renderer, sprites[i], ASSETS_PATH + "test.bmp");
    }
}

void drawSprites(SDL_Renderer *renderer) {

    for (int i = 0; i < sprites.size(); i++) {
        Sprite &sprite = sprites[i];
        if (sprite.show) {
            drawSprite(renderer, &sprite);
        }
    }
}

void drawSpritePanels(SDL_Renderer *renderer) {
    drawSpritePanelBase(renderer, &spritePanel);
    for (int i = 0; i < sprites.size(); i++) {
        Sprite &sprite = sprites[i];
        drawSpritePanelItem(renderer, &spritePanel, &sprite, i, panelSelectedIndex);
    }
}

//this function will be called in the main file before the while loop
void initBase(SDL_Renderer *renderer) {
    font = loadFont();
    initStage(&stage);
    initSprites(renderer);
    initSpritePanel(&spritePanel);
    initTopBar(&topBar);
    initPropertyPanel(renderer, &propertyPanel, windowConfig.width, windowConfig.height);
}

void engineDraw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderClear(renderer);

    drawStage(renderer, &stage);
    drawSprites(renderer);
    drawTopBar(renderer, &topBar, font);
    if (showSpritePanel) {
        drawSpritePanels(renderer);
    }
    if (activeSprite != nullptr) {
        drawPropertyPanel(renderer, &propertyPanel, activeSprite, font);
    }

    SDL_RenderPresent(renderer);
}