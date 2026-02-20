//
// Created by jack on 2/1/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include <string>

struct WindowConfig{
    int width = 800;
    int height = 600;
};



extern WindowConfig windowConfig;
extern std::string ASSETS_PATH;
constexpr int PANEL_ITEM_HEIGHT = 60;
constexpr int PANEL_ITEM_MARGIN = 10;
