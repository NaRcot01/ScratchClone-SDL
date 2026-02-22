//
// Created by jack on 2/22/2026.
//

#include "log_panel.h"
#include "../core/config.h"

void initLogPanel(LogPanel* panel) {
    panel->rect = {0, int(windowConfig.height * 0.7), windowConfig.width, int(windowConfig.height * 0.3)};
}

void drawLogPanel(SDL_Renderer* renderer, LogPanel* panel, const std::vector<std::string>& logs, TTF_Font* font) {

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 230);
    SDL_RenderFillRect(renderer, &panel->rect);
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderDrawRect(renderer, &panel->rect);

    int current_y = panel->rect.y + 5;
    for (const auto& log_line : logs) {
        drawText(renderer, font, log_line, panel->rect.x + 5, current_y, {255, 255, 255, 255});
        current_y += 20;
        if (current_y > panel->rect.y + panel->rect.h - 20) break;
    }
}