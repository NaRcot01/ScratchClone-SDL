//
// Created by mehdi on 2/13/2026.
//

#include "physics.h"

bool checkCollision(const Sprite& a, const Sprite& b) {
	return SDL_HasIntersection(&a.rect, &b.rect);
}

bool checkPointCollision(const Sprite& s, int x, int y) {
	SDL_Point p = {x,y};
	return SDL_PointInRect(&p, &s.rect);
}
