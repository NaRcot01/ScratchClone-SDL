//
// Created by jack on 2/2/2026.
//
#include "sprite.h"
#include "../tools/tools.h"

const int spriteHeight = 160;
const int spriteWidth = 160;

void initSprite(Sprite &sprite, const Stage *stage) {
    sprite.rect.x = stage->rect.x * 1.1;
    sprite.rect.y = stage->rect.y * 1.1;
    sprite.rect.h = spriteHeight;
    sprite.rect.w = spriteWidth;
    sprite.texture = NULL;
    sprite.selected = false;
    sprite.dragging = false;
    sprite.diff_x_mouse = 0;
    sprite.diff_y_mouse = 0;
    sprite.rotation = 0;
    sprite.size = 100;
    sprite.name = generateRandomName();
    sprite.show = true;

}

void updateSpriteSize(Sprite *sprite) {
    sprite->rect.h = sprite->size / 100.0 * spriteHeight;
    sprite->rect.w = sprite->size / 100.0 * spriteWidth;
}

void drawSprite(SDL_Renderer *renderer, const Sprite *sprite) {

    if (sprite->texture) {
        SDL_Point center = {sprite->rect.w / 2, sprite->rect.h / 2};
        SDL_RenderCopyEx(renderer, sprite->texture, nullptr, &sprite->rect, sprite->rotation,&center,SDL_FLIP_NONE);
    } else {
        SDL_SetRenderDrawColor(renderer, 18, 100, 100, 255);
        SDL_RenderFillRect(renderer, &sprite->rect);
    }
}

bool isSpriteClicked(int x, int y, const Sprite *sprite) {
    SDL_Point p = {x, y};
    return SDL_PointInRect(&p, &sprite->rect);
}

void moveSprite(Sprite *sprite, int x, int y, const Stage *stage, bool differential) {
    if (differential) {
        sprite->rect.x += x;
        sprite->rect.y += y;
    } else {
        sprite->rect.x = x;
        sprite->rect.y = y;
    }


    spriteValidate(sprite, stage);

}

bool loadSpriteTexture(SDL_Renderer *renderer, Sprite &sprite, std::string path) {
    SDL_Surface *surface = SDL_LoadBMP(path.c_str());
    if (!surface) {
        return false;
    }

    sprite.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return sprite.texture != NULL;
}

void spriteValidate(Sprite *sprite, const Stage *stage) {
    if (sprite->rect.w >= 480) {
        sprite->size = 300;
        updateSpriteSize(sprite);
    }
    if (sprite->rect.h >= 480) {
        sprite->size = 300;
        updateSpriteSize(sprite);
    }
    if (sprite->rect.x < stage->rect.x) {
        sprite->rect.x = stage->rect.x;
    }
    if (sprite->rect.y < stage->rect.y) {
        sprite->rect.y = stage->rect.y;
    }
    if (sprite->rect.x + sprite->rect.w > stage->rect.x + stage->rect.w) {
        sprite->rect.x = stage->rect.x + stage->rect.w - sprite->rect.w;
    }
    if (sprite->rect.y + sprite->rect.h > stage->rect.y + stage->rect.h) {
        sprite->rect.y = stage->rect.y + stage->rect.h - sprite->rect.h;
    }
    if(sprite->rotation >= 360){
        sprite->rotation = sprite->rotation % 360;
    }
}