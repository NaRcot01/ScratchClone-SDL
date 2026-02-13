//
// Created by mehdi on 2/13/2026.
//

#pragma once
#include <SDL2/SDL.h>
#include "../ui/sprite.h"


bool checkCollision(const Sprite& a, const Sprite& b);

bool checkPointCollision(const Sprite& s, int x,int y);