//
// Created by jack on 2/1/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>

struct WindowConfig{
    int width = 1600;
    int height = 800;
};


extern WindowConfig windowConfig;
extern std::string ASSETS_PATH;
extern std::vector<std::string> libraryFiles;
extern std::vector<std::string> backdrop_library_files;
constexpr int PANEL_ITEM_HEIGHT = 60;
constexpr int PANEL_ITEM_MARGIN = 10;
constexpr int BLOCK_SPACING = 5;