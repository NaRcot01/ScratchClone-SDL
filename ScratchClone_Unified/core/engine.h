//
// Created by jack on 1/31/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "../ui/sprite.h"
#include "../ui/stage.h"
#include "../ui/sprite_panel.h"
#include "../ui/topbar.h"
#include "../ui/property_panel.h"

struct Engine{
    bool running;
    Stage stage;
    std::vector<Sprite> sprites;
    SpritePanel spritePanel;
    TopBar topBar;
    PropertyPanel propertyPanel;

    Sprite* activeSprite = nullptr;
    int panelSelectedIndex = -1;
    bool showSpritePanel = false;
   
    TTF_Font* font = nullptr;

    bool isRunningScripts = false;
    bool isPaused = false;
};

void initBase(Engine &engine, SDL_Renderer *renderer);
void engineInit(Engine &engine);
void engineHandleEvents(Engine &engine, SDL_Event& event);
void engineUpdate(Engine &engine);
void engineDraw(Engine &engine, SDL_Renderer* renderer);

void drawSprites(SDL_Renderer *renderer, const std::vector<Sprite>& sprites);
void drawSpritePanels(SDL_Renderer *renderer, const SpritePanel *panel, const std::vector<Sprite>& sprites, int PanelSelectedIndex);