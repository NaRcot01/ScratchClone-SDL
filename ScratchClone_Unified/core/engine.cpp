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

void updateScript(Script &script , Sprite &sprite , double deltaTime , std::vector<std::string> &logs);

void startFlagScripts(Engine &engine) {
    for (auto &sprite : engine.sprites) {
	for (auto &script : sprite.scripts) {
		if (script.startEvent == BlockType::ON_FLAG_CLICKED) {
			script.pc = 0;
			script.waitTimer = 0.0;
			script.loopStack.clear();
			script.isActive = true;
			}
		}
	}
	engine.isRunningScripts = true;
}

void stopAllScripts(Engine &engine) {
	for (auto &sprite : engine.sprites) {
		for (auto &script : sprite.scripts) {
			script.isActive = false;
		}
	}
	engine.isRunningScripts = false;
}

void engineInit(Engine &engine, SDL_Event& event) {
    (void)event;
    engine.running = true;
    engine.isRunningScripts = false;
    engine.isPaused = false;
}

void engineHandleEvents(Engine &engine, SDL_Event& event) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            engine.running = false;
        }

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            windowConfig.width = event.window.data1;
            windowConfig.height = event.window.data2;
            resizeStage(&engine.stage);
        }

        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
            if (engine.activeSprite) {
                engine.activeSprite->dragging = false;

            }
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE) {
            for (auto &row: engine.propertyPanel.rows) {
                if (row.active && !row.value.empty()) {
                    row.value.pop_back();
                    applyPropertyToSprite(row, engine.activeSprite, &engine.stage);
                }
            }
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
            for (auto &row: engine.propertyPanel.rows) {
                if (!row.active) { continue; }
                applyPropertyToSprite(row, engine.activeSprite, &engine.stage);
                row.active = false;
            }
        } else if (event.type == SDL_KEYDOWN && engine.propertyPanel.visible && engine.activeSprite) {
            for (auto &row: engine.propertyPanel.rows) {
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
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_F5) {
				startFlagScripts(engine);
			} else if (event.key.keysym.sym == SDLK_F6) {
				stopAllScripts(engine);
			} else if (event.key.keysym.sym == SDLK_F7) {
				engine.isPaused = !engine.isPaused;
			}
		}
                applyPropertyToSprite(row, engine.activeSprite, &engine.stage);
            }
        }

        if (event.type == SDL_MOUSEMOTION) {
            if (engine.activeSprite && engine.activeSprite->dragging) {
                moveSprite(engine.activeSprite, event.motion.x - engine.activeSprite->diff_x_mouse, event.motion.y - engine.activeSprite->diff_y_mouse, &engine.stage);
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            if (engine.activeSprite) {
                spriteValidate(engine.activeSprite, &engine.stage);  // it will check if the X,Y of the sprite is correct
            }
            bool clickOnPanel = false;

            int m_x = event.button.x;
            int m_y = event.button.y;

            if (engine.propertyPanel.visible && handlePropertyPanelClicked(&engine.propertyPanel, engine.activeSprite, m_x, m_y)) {
                return; // mouse is clicked on property panel so there is no need to check other conditions.
            }

            for (int i = 0; i < engine.sprites.size(); i++) {
                Sprite &sprite = engine.sprites[i];

                SDL_Rect itemRect = {
                        engine.spritePanel.rect.x + 10,
                        engine.spritePanel.rect.y + 10 + i * (PANEL_ITEM_HEIGHT + PANEL_ITEM_MARGIN) + 20,
                        engine.spritePanel.rect.w - 20,
                        PANEL_ITEM_HEIGHT
                };

                if (m_x >= itemRect.x &&
                    m_x <= itemRect.x + itemRect.w &&
                    m_y >= itemRect.y &&
                    m_y <= itemRect.y + itemRect.h &&
                    engine.showSpritePanel) {
                    engine.panelSelectedIndex = i;
                    clickOnPanel = true;
                    engine.activeSprite = &engine.sprites[i];
                    engine.propertyPanel.visible = true;
                    engine.activeSprite->selected = true;
                    engine.activeSprite->dragging = false;
                    for (int j = 0; j < engine.sprites.size(); j++) { //make sure other sprites are deselected
                        if (i != j) {
                            engine.sprites[j].selected = false;
                        }
                    }
                    break;
                }
            }
            if (!clickOnPanel) {
                engine.activeSprite = nullptr;
                engine.panelSelectedIndex = -1;
                engine.propertyPanel.visible = false;

                for (int i = 0; i < engine.sprites.size(); i++) {
                    Sprite &sprite = engine.sprites[i];
                    sprite.selected = false;
                    sprite.dragging = false;
                    if (isSpriteClicked(m_x, m_y, &sprite)) {
                        sprite.selected = true;
                        sprite.dragging = true;
                        std::cout << " sprite clicked " << std::endl;
                        sprite.diff_x_mouse = m_x - sprite.rect.x;
                        sprite.diff_y_mouse = m_y - sprite.rect.y;
                        engine.activeSprite = &sprite;
                        engine.propertyPanel.visible = true;
                        engine.panelSelectedIndex = i;
                        break;
                    }
                }
            }

            for (int i = 0; i < engine.topBar.buttonCount; i++) {
                if (engine.topBar.buttons[i].isClicked(m_x, m_y)) {
                    if (engine.topBar.buttons[i].type == BTN_SPRITE_PANEL) {
                        engine.showSpritePanel = !engine.showSpritePanel;
                    }
                }
            }
        }
    }
}

void engineUpdate(Engine &engine) {

static Uint32 lastTime = 0;
Uint32 currentTime = SDL_GetTicks();
if (lastTime == 0) lastTime = currentTime;
double deltaTime = (currentTime - lastTime)/1000.0;
lastTime=currentTime;

std::vector<std::string> logs;

for (auto &sprite : engine.sprites) {
     if (engine.isRunningScripts && !engine.isPaused) {
	for (auto &script : sprite.scripts){
		updateScript(script, sprite, deltaTime, logs);
		}
	}
	spriteValidate(&sprite, &engine.stage);
      }
	for(const auto& log : logs) {std::cout << log << std::endl;}
}


void drawSprites(SDL_Renderer *renderer , const std::vector<Sprite>& sprites) {
	for (const auto& sprite : sprites) {
		if(sprite.isVisible) {drawSprite(renderer, &sprite);}
	}
}

void drawSpritePanels(SDL_Renderer *renderer , const SpritePanel& panel, const std::vector<Sprite>& sprites, int panelSelectedIndex) {
	drawSpritePanelBase(renderer, &panel);
	for (int i=0; i<sprites.size(); i++) {
	     drawSpritePanel(renderer, &panel, &sprites[i], i, panelSelectedIndex);
	}
}

void initSprites(Engine& engine) {
	engine.sprites.resize(3);
	for(int i=0; i< (int)engine.sprites.size(); i++) {
		initSprite(engine.sprites[i], &engine.stage);
		engine.sprites[i].rect.x +=50*i;
		engine.sprites[i].exact_x = (double)engine.sprites[i].rect.x;
		engine.sprites[i].exact_y = (double)engine.sprites[i].rect.y;
	}
	
	Script testScript;
	testScript.isActive = false;
	testScript.pc = 0;
	testScript.startEvent = BlockType::ON_FLAG_CLICKED;
	

	Block bMove; bMove.type = BlockType::MOVE; bMove.parameters.push_back(2.0);
	testScript.blocks.push_back(bMove);

	Block bTurn; bTurn.type = BlockType::TURN_RIGHT; bTurn.parameters.push_back(1.0);
	testScript.blocks.push_back(bTurn);

	Block bLoop; bLoop.type = BlockType::FOREVER; bLoop.jumpToIndex = 0;
	testScript.blocks.push_back(bLoop);
        
	Block bForever; bForever.type = BlockType::FOREVER;
	testScript.blocks.push_back(bForever);

	preprocessScript(testScript.blocks);

	if (!testScript.blocks.empty()) {
		int foreverIndex = (int)testScript.blocks.size() - 1;
		testScript.blocks[foreverIndex].jumpToIndex = 0;
	}

	engine.sprites[0].scripts.clear();
	engine.sprites[0].scripts.push_back(testScript);
}

void initBase(Engine& engine, SDL_Renderer *renderer) {
	engine.font = loadFont();
	initStage(&engine.stage);
	initSprites(engine);
	initSpritePanel(&engine.spritePanel);
	initTopBar(&engine.topBar);
	for (auto &sprite: engine.sprites) {
		loadSpriteTexture(renderer, sprite, ASSETS_PATH + "test.bmp");
	}
	initPropertyPanel(&engine.propertyPanel, windowConfig.width, windowConfig.height);
}

void engineDraw(Engine& engine, SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
	SDL_RenderClear(renderer);

	drawStage(renderer, &engine.stage);
	drawSprites(renderer, engine.sprites);
	drawTopBar(renderer, &engine.topBar, engine.font);

	if (engine.showSpritePanel) {
		drawSpritePanels(renderer, engine.spritePanel, engine.sprites, engine.panelSelectedIndex);
	}
	if (engine.activeSprite !=nullptr) {
		drawPropertyPanel(renderer, &engine.propertyPanel, engine.activeSprite, engine.font);
	}

	SDL_RenderPresent(renderer);
}
