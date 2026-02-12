//
// Created by jack on 2/1/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include <string>

struct WindowConfig{
    int width = 1600;
    int height = 800;
};



extern WindowConfig windowConfig;
extern std::string ASSETS_PATH;
constexpr int PANEL_ITEM_HEIGHT = 60;
constexpr int PANEL_ITEM_MARGIN = 10;
