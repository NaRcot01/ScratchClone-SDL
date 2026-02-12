//
// Created by jack on 2/5/2026.
//

#pragma once

#include <SDL2/SDL.h>
#include "sprite.h"
#include "font.h"

//enum PropertyType{
//    PROPERTY_NAME,
//    PROPERTY_X,
//    PROPERTY_Y,
//    PROPERTY_SIZE,
//    PROPERTY_ROTATION
//};     WE ARE NOT PERMITTED TO USE ENUM, SO WE BASICALLY JUST REPLACE IT WITH STRING. AND THE TYPES ARE AS MENTIONED IN THIS COMMENT !

const std::string propertyType[5] = {
        "PROPERTY_NAME",
        "PROPERTY_X",
        "PROPERTY_Y",
        "PROPERTY_SIZE",
        "PROPERTY_ROTATION",
};

struct PropertyRow {
    std::string label;
    std::string value;
    SDL_Rect inputRect;
    std::string type;
    bool active;
};


struct PropertyPanel {
    SDL_Rect rect;
    PropertyRow rows[5];
    bool visible;

    SDL_Rect visibilityBtnRect;
    SDL_Texture *eyeOpenTexture;
    SDL_Texture *eyeCloseTexture;
    bool visibilityBtnState; // 0 means its hidden and 1 means its shown.
};


void initPropertyPanel(SDL_Renderer *renderer, PropertyPanel *panel, int screenWidth, int screenHeight);

void drawPropertyPanel(SDL_Renderer *renderer, PropertyPanel *panel, Sprite *activeSprite, TTF_Font *font);

bool handlePropertyPanelClicked(PropertyPanel *panel, Sprite *activeSprite, int m_x, int m_y);

void applyPropertyToSprite(PropertyRow &row, Sprite *sprite, Stage *stage);