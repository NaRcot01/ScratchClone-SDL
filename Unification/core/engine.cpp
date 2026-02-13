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

            if (propertyPanel.visible && handlePropertyPanelClicked(&propertyPanel, activeSprite, m_x, m_y)) {
                return; // mouse is clicked on property panel so there is no need to check other conditions.
            }

            for (int i = 0; i < sprites.size(); i++) {
                Sprite &sprite = sprites[i];

                SDL_Rect itemRect = {
                        spritePanel.rect.x + 10,
                        spritePanel.rect.y + 10 + i * (PANEL_ITEM_HEIGHT + PANEL_ITEM_MARGIN) + 20,
                        spritePanel.rect.w - 20,
                        PANEL_ITEM_HEIGHT
                };

                if (m_x >= itemRect.x &&
                    m_x <= itemRect.x + itemRect.w &&
                    m_y >= itemRect.y &&
                    m_y <= itemRect.y + itemRect.h &&
                    showSpritePanel) {
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
            if (!clickOnPanel) {
                activeSprite = NULL;
                panelSelectedIndex = -1;
                propertyPanel.visible = false;

                for (int i = 0; i < sprites.size(); i++) {
                    Sprite &sprite = sprites[i];
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

void updateScript(Script &script , Sprite &sprite , double deltaTime , std::vector<std::string> &logs);

void engineUpdate() {

static Uint32 lastTime = 0;
Uint32 currentTime = SDL_GetTicks();
if (lastTime == 0) lastTime = currentTime;
double deltaTime = (currentTime - lastTime)/1000.0;
lastTime=currentTime;

std::vector<std::string> logs;
for (auto &sprite : sprites) {
	for (auto &script : sprite.scripts){
		updateScript(script, sprite, deltaTime, logs);
	}
	spriteValidate(&sprite, &stage);
	}
	
	for(const auto& log : logs) {
		std::cout << log << std::endl;
	}
}


void initSprites() {
    sprites.resize(3); // should be removed.    JUST FOR TEST
    for (int i = 0; i < sprites.size(); i++) {
        initSprite(sprites[i], &stage);
        sprites[i].rect.x += 50 * i;

	sprites[i].exact_x = sprites[i].rect.x;
	sprites[i].exact_y = sprites[i].rect.y;
    }
	
	Script testScript;
	testScript.isActive = true;

	Block bMove; bMove.type = BlockType::MOVE; bMove.parameters.push_back(2.0);
	testScript.blocks.push_back(bMove);

	Block bTurn; bTurn.type = BlockType::TURN_RIGHT; bTurn.parameters.push_back(1.0);
	testScript.blocks.push_back(bTurn);

	Block bLoop; bLoop.type = BlockType::FOREVER; bLoop.jumpToIndex = 0;
	testScript.blocks.push_back(bLoop);

	sprites[0].scripts.push_back(testScript);
}

void drawSprites(SDL_Renderer *renderer) {

    for (int i = 0; i < sprites.size(); i++) {
        Sprite &sprite = sprites[i];
        drawSprite(renderer, &sprite);
    }
}

void drawSpritePanels(SDL_Renderer *renderer) {
    drawSpritePanelBase(renderer, &spritePanel);
    for (int i = 0; i < sprites.size(); i++) {
        Sprite &sprite = sprites[i];
        drawSpritePanel(renderer, &spritePanel, &sprite, i, panelSelectedIndex);
    }
}

//this function will be called in the main file before the while loop
void initBase(SDL_Renderer *renderer) {
    font = loadFont();
    initStage(&stage);
    initSprites();
    initSpritePanel(&spritePanel);
    initTopBar(&topBar);
    for (auto &sprite: sprites) {
        loadSpriteTexture(renderer, sprite, ASSETS_PATH + "test.bmp");
    }
    initPropertyPanel(&propertyPanel, windowConfig.width, windowConfig.height);
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