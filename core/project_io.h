//
// Created by jack on 2/21/2026.
//

#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include "../ui/sprite.h"
#include "../ui/stage.h"
#include "config.h"

bool saveProject(const std::string& path);
bool loadProject(const std::string& path, SDL_Renderer* renderer);
