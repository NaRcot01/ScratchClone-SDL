//
// Created by jack on 2/5/2026.
//


#include "property_panel.h"
#include "sprite.h"
#include "font.h"
#include "../core/config.h"

void initPropertyPanel(SDL_Renderer *renderer, PropertyPanel *panel, int screenWidth, int screenHeight) {

    panel->visible = false;
    panel->rect.x = 0.525 * screenWidth;
    panel->rect.y = 0.04 * screenHeight;
    panel->rect.h = screenHeight / 3;
    panel->rect.w = screenWidth * 0.125;

    std::string labels[5] = {
            "name",
            "x", "y",
            "size",
            "rotation"
    };

    // making it responsive
    int paddingX = panel->rect.w * 0.05;
    int paddingY = panel->rect.h * 0.06;
    int rowHeight = panel->rect.h / 6;

    int labelWidth = panel->rect.w * 0.35;
    int inputWidth = panel->rect.w * 0.55;
    int inputHeight = rowHeight * 0.6;


    for (int i = 0; i < 5; i++) {
        panel->rows[i].label = labels[i];
        panel->rows[i].value = "";
        panel->rows[i].active = false;
        panel->rows[i].type = propertyType[i];

        int rowY = panel->rect.y + paddingY + i * rowHeight;

        panel->rows[i].inputRect = {
                panel->rect.x + paddingX + labelWidth,
                rowY + (rowHeight - inputHeight) / 2,
                inputWidth, inputHeight
        };
    }

    SDL_Surface *surface1 = SDL_LoadBMP((ASSETS_PATH + "icons/eyeopen.bmp").c_str());
    SDL_Surface *surface2 = SDL_LoadBMP((ASSETS_PATH + "icons/eyeclose.bmp").c_str());

    if (!surface1 || !surface2) {
        // log : error while loading eyeclose and eyeopen icons!
    }

    panel->eyeOpenTexture = SDL_CreateTextureFromSurface(renderer, surface1);
    panel->eyeCloseTexture = SDL_CreateTextureFromSurface(renderer, surface2);

    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);

    panel->visibilityBtnRect.w = panel->rect.w * 0.15;
    panel->visibilityBtnRect.h = panel->rect.h * 0.12;
    panel->visibilityBtnRect.x = panel->rect.x + panel->rect.w - panel->visibilityBtnRect.w - 90;
    panel->visibilityBtnRect.y = panel->rect.y + panel->rect.h - panel->visibilityBtnRect.h - 5;

    panel->flip_h_button_rect = {
            panel->visibilityBtnRect.x + panel->visibilityBtnRect.w + 10,
            panel->visibilityBtnRect.y,
            panel->visibilityBtnRect.w,
            panel->visibilityBtnRect.h
    };
    panel->flip_v_button_rect = {
            panel->flip_h_button_rect.x + panel->flip_h_button_rect.w + 10,
            panel->visibilityBtnRect.y,
            panel->visibilityBtnRect.w,
            panel->visibilityBtnRect.h
    };
    SDL_Surface* surface_flip_h = IMG_Load((ASSETS_PATH + "icons/flip_v.jpg").c_str());
    if (surface_flip_h) {
        panel->flip_h_button_texture = SDL_CreateTextureFromSurface(renderer, surface_flip_h);
        SDL_FreeSurface(surface_flip_h);
    }

    SDL_Surface* surface_flip_v = IMG_Load((ASSETS_PATH + "icons/flip_h.jpg").c_str());
    if (surface_flip_v) {
        panel->flip_v_button_texture = SDL_CreateTextureFromSurface(renderer, surface_flip_v);
        SDL_FreeSurface(surface_flip_v);
    }

}

void drawPropertyRow(SDL_Renderer *renderer, TTF_Font *font, PropertyRow *row, int x, int y) {

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gray = {128, 128, 128, 255};
    SDL_Color yellow = {255, 200, 100, 255};
    SDL_Color green = {123, 210, 51, 255};

    SDL_Color border = row->active ? green : gray;
    int borderThickness = row->active ? 3 : 1;


    // draw label. i.g: name,x,y....
    drawText(renderer, font, row->label, x, y, yellow);

    //input box BG
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
    SDL_RenderFillRect(renderer, &row->inputRect);

    //input box border. color changed if it's activated with more thickness.
    SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
    for (int i = 0; i < borderThickness; i++) {
        SDL_Rect rect = {
                row->inputRect.x - i,
                row->inputRect.y - i,
                row->inputRect.w + 2 * i,
                row->inputRect.h + 2 * i,

        };
        SDL_RenderDrawRect(renderer, &rect);
    }

    //text inside the input box
    if (!row->value.empty()) {
        drawText(renderer, font, row->value, x + 75, y, gray);
    }


}

void drawPropertyPanel(SDL_Renderer *renderer, PropertyPanel *panel, Sprite *activeSprite, TTF_Font *font) {
    SDL_Color black = {255, 255, 255, 255};
    if (!panel->visible || !activeSprite) {
        return;
    }
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(renderer, &panel->rect);


    panel->rows[0].value = activeSprite->name;
    panel->rows[1].value = std::to_string(activeSprite->rect.x - 1040); // mokhtasat nesbet be stage dade mishe
    panel->rows[2].value = std::to_string(activeSprite->rect.y - 32);  // mokhtasat nesbet be stage dade mishe
    panel->rows[3].value = std::to_string(activeSprite->size);
    panel->rows[4].value = std::to_string(activeSprite->rotation);

    int paddingX = panel->rect.w * 0.05;
    int paddingY = panel->rect.h * 0.06;
    int rowHeight = panel->rect.h / 6;


    for (int i = 0; i < 5; i++) {
        int x = panel->rect.x + paddingX;
        int y = panel->rect.y + paddingY + i * rowHeight + 10;

        drawPropertyRow(renderer, font, &panel->rows[i], x, y);
    }

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer,&panel->visibilityBtnRect);

    if (activeSprite->show) {
        SDL_RenderCopy(renderer, panel->eyeOpenTexture, nullptr, &panel->visibilityBtnRect);
        panel->visibilityBtnState = true;
    } else {
        SDL_RenderCopy(renderer, panel->eyeCloseTexture, nullptr, &panel->visibilityBtnRect);
        panel->visibilityBtnState = false;
    }

    if (panel->flip_h_button_texture) {
        SDL_RenderCopy(renderer, panel->flip_h_button_texture, NULL, &panel->flip_h_button_rect);
    }
    if (panel->flip_v_button_texture) {
        SDL_RenderCopy(renderer, panel->flip_v_button_texture, NULL, &panel->flip_v_button_rect);
    }

}

bool handlePropertyPanelClicked(PropertyPanel *panel, Sprite *activeSprite, int m_x, int m_y) {
    SDL_Point p = {m_x, m_y};
    bool isClicked = false;
    for (int i = 0; i < 5; i++) {
        if (SDL_PointInRect(&p, &panel->rows[i].inputRect)) {
            panel->rows[i].active = true;
            isClicked = true;
        } else {
            panel->rows[i].active = false;
        }
    }

    if(SDL_PointInRect(&p, &panel->visibilityBtnRect)){
        isClicked = true;
        panel->visibilityBtnState = !panel->visibilityBtnState;
        activeSprite->show = !activeSprite->show;
    }
    if(SDL_PointInRect(&p, &panel->flip_h_button_rect)) {
        isClicked = true;
        // if it is already flipped, deactivate flip mode
        if (activeSprite->flip_mode & SDL_FLIP_HORIZONTAL) {
            activeSprite->flip_mode = (SDL_RendererFlip)(activeSprite->flip_mode & ~SDL_FLIP_HORIZONTAL);
        }
        // flip it
        else {
            activeSprite->flip_mode = (SDL_RendererFlip)(activeSprite->flip_mode | SDL_FLIP_HORIZONTAL);
        }
    }
    if(SDL_PointInRect(&p, &panel->flip_v_button_rect)) {
        isClicked = true;
        // if it is already flipped, deactivate flip mode
        if (activeSprite->flip_mode & SDL_FLIP_VERTICAL) {
            activeSprite->flip_mode = (SDL_RendererFlip)(activeSprite->flip_mode & ~SDL_FLIP_VERTICAL);
        }
        // flip it
        else {
            activeSprite->flip_mode = (SDL_RendererFlip)(activeSprite->flip_mode | SDL_FLIP_VERTICAL);
        }
    }
    if (SDL_PointInRect(&p, &panel->rect)) { isClicked = true; }

    return isClicked;

}


void applyPropertyToSprite(PropertyRow &row, Sprite *sprite, Stage *stage) {
    char integerDefaultValue = '0';
    if (row.value == "" && row.type != "PROPERTY_NAME") { row.value = integerDefaultValue; }

    try {
        if (row.type == "PROPERTY_NAME") {
            sprite->name = row.value;
            // log :
        } else if (row.type == "PROPERTY_X") {
            sprite->rect.x = std::stoi(row.value) + 1040;
            // log :
        } else if (row.type == "PROPERTY_Y") {
            sprite->rect.y = std::stoi(row.value) + 32;
            // log :
        } else if (row.type == "PROPERTY_SIZE") {
            sprite->size = std::stoi(row.value);
            updateSpriteSize(sprite);
            spriteValidate(sprite, stage);
            // log :
        } else if (row.type == "PROPERTY_ROTATION") {
            sprite->rotation = std::stoi(row.value);
            // log :
        }
    }
    catch (...) {
        // log : error while updating sprite info
    }
}