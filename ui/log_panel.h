//
// Created by jack on 2/22/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "font.h"

struct LogPanel {
    SDL_Rect rect;
};

void initLogPanel(LogPanel* panel);
void drawLogPanel(SDL_Renderer* renderer, LogPanel* panel, const std::vector<std::string>& logs, TTF_Font* font);