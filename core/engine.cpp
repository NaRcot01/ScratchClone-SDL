//
// Created by jack on 1/31/2026.
//

#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "engine.h"
#include "config.h"
#include "../ui/sprite_panel.h"
#include "../ui/sprite.h"
#include "../ui/stage.h"
#include "../ui/topbar.h"
#include "../ui/property_panel.h"
#include "../tools/tinyfiledialogs.h"
#include "../ui/library_panel.h"
#include "../tools/tools.h"
#include "../ui/script_area.h"
#include "../ui/block_palette.h"
#include "../ui/control_panel.h"

int panelSelectedIndex = -1;


SDL_Event event;
Stage stage;
std::vector<Sprite> sprites;
SpritePanel spritePanel;
Sprite *activeSprite = NULL;
bool showSpritePanel = false;
TopBar topBar;
TTF_Font *font;
PropertyPanel propertyPanel;
LibraryPanel libraryPanel;
bool showLibraryPanel = false;
ScriptArea scriptArea;
BlockPalette blockPalette;
Block *active_editing_block = nullptr;
int active_editing_param_index = -1;
std::string original_value_on_edit;
Block* dragged_block = nullptr;
ControlPanel controlPanel;
int drag_offset_x = 0;
int drag_offset_y = 0;


void engineInit(Engine &engine) {
    engine.running = true;
}

void engineHandleEvents(Engine &engine, SDL_Renderer *renderer) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            engine.running = false;
            // log :
        }

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            int W = event.window.data1;
            int H = event.window.data2;

            windowConfig.height = H;
            windowConfig.width = W;
            // log :
            resizeStage(&stage);
        }

        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
            if (activeSprite) {
                activeSprite->dragging = false;
            }
            if (dragged_block) {
                SDL_Point mouse_point = {event.button.x, event.button.y};

                bool block_snapped = false;

                if (activeSprite && SDL_PointInRect(&mouse_point, &scriptArea.rect)) {

                    for (auto& script : activeSprite->scripts) {
                        if (script.empty()) continue;

                        Block& last_block = script.back();

                        SDL_Rect snap_zone = {
                                last_block.rect.x,
                                last_block.rect.y + last_block.rect.h,
                                last_block.rect.w,
                                40
                        };

                        if (SDL_PointInRect(&mouse_point, &snap_zone)) {
                            script.push_back(*dragged_block);
                            block_snapped = true;
                            break;
                        }
                    }
                    if (!block_snapped) {
                        std::vector<Block> new_script;
                        new_script.push_back(*dragged_block);
                        activeSprite->scripts.push_back(new_script);
                    }
                }
                delete dragged_block;
                dragged_block = nullptr;
            }
        }
        if (active_editing_block && event.type == SDL_KEYDOWN) {
            SDL_Keycode key = event.key.keysym.sym;


            if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
                if (active_editing_param_index < active_editing_block->parameters.size() &&
                    active_editing_block->parameters[active_editing_param_index] == 0) {
                    if (original_value_on_edit != "")
                        active_editing_block->parameters[active_editing_param_index] = std::stod(
                                original_value_on_edit);
                }

                active_editing_block = nullptr;
                active_editing_param_index = -1;
                return;
            }    ParamType current_param_type = ParamType::NUMERIC;
            if (block_styles.count(active_editing_block->type)) {
                const auto& param_types = block_styles[active_editing_block->type].param_types;
                if (active_editing_param_index < param_types.size()) {
                    current_param_type = param_types[active_editing_param_index];
                }
            }

            if (current_param_type == ParamType::NUMERIC) {
                if (key == SDLK_BACKSPACE && active_editing_block->parameters[active_editing_param_index] != 0) {
                    active_editing_block->parameters[active_editing_param_index] = (int)active_editing_block->parameters[active_editing_param_index] / 10;
                } else if (key >= SDLK_0 && key <= SDLK_9) {
                    int entered_digit = key - SDLK_0;
                    active_editing_block->parameters[active_editing_param_index] = active_editing_block->parameters[active_editing_param_index] * 10 + entered_digit;
                }
            }
            else if (current_param_type == ParamType::STRING) {
                if (key == SDLK_BACKSPACE && !active_editing_block->textParam.empty()) {
                    active_editing_block->textParam.pop_back();
                } else if ((key >= SDLK_a && key <= SDLK_z) || (key >= SDLK_0 && key <= SDLK_9) || key == SDLK_SPACE) {
                    char typed_char = (char)key;
                    if (SDL_GetModState() & KMOD_SHIFT) {
                        if (typed_char >= 'a' && typed_char <= 'z') typed_char -= 32;

                    }
                    active_editing_block->textParam += typed_char;
                }
            }

        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE) {
            for (auto &row: propertyPanel.rows) {
                if (row.active && !row.value.empty()) {
                    row.value.pop_back();
                    applyPropertyToSprite(row, activeSprite, &stage);
                }
            }
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
            for (auto &row: propertyPanel.rows) {
                if (!row.active) { continue; }

                applyPropertyToSprite(row, activeSprite, &stage);
                spriteValidate(activeSprite, &stage);
                row.active = false;
            }
        } else if (event.type == SDL_KEYDOWN && propertyPanel.visible && activeSprite) {
            for (auto &row: propertyPanel.rows) {
                if (!row.active) { continue; }

                char c = event.key.keysym.sym;

                if (row.type != "PROPERTY_NAME") {
                    if (c >= SDLK_0 && c <= SDLK_9) {
                        row.value += char('0' + c - SDLK_0);
                    }
                } else {
                    if ((c >= SDLK_a && c <= SDLK_z) || (c >= SDLK_0 && c <= SDLK_9) || (c == SDLK_UNDERSCORE)) {
                        row.value += char('a' + c - SDLK_a);
                    }
                }
                applyPropertyToSprite(row, activeSprite, &stage);
            }
        }

        if (event.type == SDL_MOUSEMOTION) {
            if (activeSprite && activeSprite->dragging) {
                int m_x = event.button.x;
                int m_y = event.button.y;
                std::cout << " dragging sprite " << std::endl;
                // log :
                moveSprite(activeSprite, m_x - activeSprite->diff_x_mouse, m_y - activeSprite->diff_y_mouse, &stage);
            }
            if (dragged_block) {
                dragged_block->rect.x = event.motion.x - drag_offset_x;
                dragged_block->rect.y = event.motion.y - drag_offset_y;
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            if (activeSprite) {
                spriteValidate(activeSprite, &stage);
            }
            bool clickOnPanel = false;

            int m_x = event.button.x;
            int m_y = event.button.y;
            SDL_Point p = {m_x, m_y};
            bool clicked_on_a_param = false;
            bool click_was_handled = false;
            if (SDL_PointInRect(&p, &blockPalette.block_panel_rect)) {
                for (auto& template_block : blockPalette.template_blocks) {
                    if (SDL_PointInRect(&p, &template_block.rect)) {
                        dragged_block = new Block(template_block);

                        drag_offset_x = m_x - template_block.rect.x;
                        drag_offset_y = m_y - template_block.rect.y;

                        click_was_handled = true;
                    }
                }
            }
            if (activeSprite) {
                for (auto &script: activeSprite->scripts) {
                    for (auto &block: script) {

                        if (!SDL_PointInRect(&p, &block.rect)) continue;

                        for (size_t i = 0; i < block.param_rects.size(); ++i) {
                            if (SDL_PointInRect(&p, &block.param_rects[i])) {

                                active_editing_block = &block;
                                active_editing_param_index = i;
                                clicked_on_a_param = true;

                                if (i < block.parameters.size()) {
                                    original_value_on_edit = std::to_string((int) block.parameters[i]);
                                } else {
                                    original_value_on_edit = block.textParam;
                                }

                                if (i < block.parameters.size()) {
                                    block.parameters[i] = 0;
                                } else {
                                    block.textParam = "";
                                }
                                click_was_handled = true;
                                break;
                            }
                        }
                    }
                }
            }
            if (SDL_PointInRect(&p, &controlPanel.green_flag_rect)) {
                engine.is_running_scripts = true;
                // log : green flag clicked. starting scripts

                click_was_handled = true;
            }
            if (SDL_PointInRect(&p, &controlPanel.stop_button_rect)) {
                engine.is_running_scripts = false;
                // log : stop button clicked! stoping all scripts
                click_was_handled = true;
            }
            if (!clicked_on_a_param) {
                active_editing_block = nullptr;
                active_editing_param_index = -1;
            }
            if (propertyPanel.visible && handlePropertyPanelClicked(&propertyPanel, activeSprite, m_x, m_y)) {
                click_was_handled = true;
                return; // mouse is clicked on property panel so there is no need to check other conditions.
            }
            if (showLibraryPanel) {
                if (SDL_PointInRect(&p, &libraryPanel.closeBtnRect)) {
                    showLibraryPanel = false;
                    return;
                }

                for (int i = 0; i < libraryPanel.itemRects.size(); i++) {
                    if (SDL_PointInRect(&p, &libraryPanel.itemRects[i])) {
                        addNewSpriteFromFile(renderer, libraryPanel.itemPaths[i].c_str(), stage, sprites);
                        showLibraryPanel = false;
                        return;
                    }
                }
                continue;
            }
            if (!click_was_handled && showSpritePanel) { // handle adding sprite through sprite panel.
                if (SDL_PointInRect(&p, &spritePanel.buttonRects[0])) { //upload btn clicked
                    const char *filterPatterns[3] = {"*.png", "*.jpg", "*.bmp"};
                    const char *filePath = tinyfd_openFileDialog(
                            "Choose a Sprite Image",
                            "",
                            3,
                            filterPatterns,
                            "Image Files(JPG, PNG, BMP)",
                            0
                    );
                    if (filePath) {
                        std::string sourcePath = filePath;
                        size_t lastSlash = sourcePath.find_last_of("/\\");
                        std::string fileName = (lastSlash == std::string::npos) ? sourcePath : sourcePath.substr(
                                lastSlash + 1);

                        std::string destPathStr = ASSETS_PATH + "sprite_lib/" + fileName;
                        if (copyFile(sourcePath, destPathStr)) {
                            // log sprite added to lib
                            libraryFiles.push_back("sprite_lib/" + fileName);
                            addItemToLibraryPanel(&libraryPanel, renderer, filePath);
                        } else {
                            // log error while adding sprite to lib
                        }
                        addNewSpriteFromFile(renderer, filePath, stage, sprites);
                    }
                    return;
                }
                if (SDL_PointInRect(&p, &spritePanel.buttonRects[1])) { // library btn clicked
                    showLibraryPanel = true;
                    return;
                }

                if (SDL_PointInRect(&p, &spritePanel.buttonRects[2])) { // random btn clicked
                    unsigned seed = time(nullptr);
                    std::mt19937 gen(seed);

                    std::uniform_int_distribution<> distrib(0, libraryFiles.size() - 1);
                    int random_idx = distrib(gen);
                    std::string random_path = ASSETS_PATH + libraryFiles[random_idx];
                    addNewSpriteFromFile(renderer, random_path.c_str(), stage, sprites);

                }
            }

            if (!click_was_handled && showSpritePanel) {
                for (int i = 0; i < sprites.size(); i++) { // checks if mouse is clicked on sprite item in sprite panel.
                    Sprite &sprite = sprites[i];

                    SDL_Rect itemRect = {
                            spritePanel.rect.x + 10,
                            spritePanel.rect.y + 50 + i * (PANEL_ITEM_HEIGHT + PANEL_ITEM_MARGIN) + 20,
                            spritePanel.rect.w - 20,
                            PANEL_ITEM_HEIGHT
                    };

                    int deleteBtnSize = PANEL_ITEM_HEIGHT * 0.5;
                    SDL_Rect deleteBtnRect = {
                            itemRect.x + itemRect.w - deleteBtnSize - 5,
                            itemRect.y + (itemRect.h - deleteBtnSize) / 2,
                            deleteBtnSize,
                            deleteBtnSize
                    };

                    if (SDL_PointInRect(&p, &deleteBtnRect)) {
                        if (activeSprite == &sprite) {
                            click_was_handled = true;
                            activeSprite = NULL;
                            panelSelectedIndex = -1;
                            propertyPanel.visible = false;
                        }

                        sprites.erase(sprites.begin() + i);
                        clickOnPanel = true;
                        break;
                    }

                    if (m_x >= itemRect.x &&
                        m_x <= itemRect.x + itemRect.w &&
                        m_y >= itemRect.y &&
                        m_y <= itemRect.y + itemRect.h) {
                        panelSelectedIndex = i;
                        clickOnPanel = true;
                        activeSprite = &sprites[i];
                        propertyPanel.visible = true;
                        activeSprite->selected = true;
                        activeSprite->dragging = false;
                        click_was_handled = true;
                        for (int j = 0; j < sprites.size(); j++) { //make sure other sprites are deselected
                            if (i != j) {
                                sprites[j].selected = false;
                            }
                        }
                        break;
                    }
                }
            }
            if (!click_was_handled && (SDL_PointInRect(&p, &blockPalette.category_menu_rect) ||
                                       SDL_PointInRect(&p, &blockPalette.block_panel_rect) ||
                                       SDL_PointInRect(&p, &scriptArea.rect))){
                click_was_handled = true;
            }
            if (!click_was_handled) {
                for (int i = 0; i < topBar.buttonCount; i++) {
                    TopBarButton &btn = topBar.buttons[i];
                    if (btn.isClicked(m_x, m_y)) {
                        if (btn.type == BTN_SPRITE_PANEL) {
                            showSpritePanel = !showSpritePanel;
                            click_was_handled = true;
                        }
                    }
                }
            }

            if (!click_was_handled) { // checks if mouse is clicked on sprite on the stage.
                for (int i = 0; i < sprites.size(); i++) {
                    Sprite &sprite = sprites[i];
                    if (!sprite.show) { continue; } // if the sprite is hidden, then it is unclickable on the stage!
                    sprite.selected = false;
                    sprite.dragging = false;
                    if (isSpriteClicked(m_x, m_y, &sprite)) {
                        click_was_handled = true;
                        sprite.selected = true;
                        sprite.dragging = true;
                        std::cout << " sprite clicked " << std::endl;
                        sprite.diff_x_mouse = m_x - sprite.rect.x;
                        sprite.diff_y_mouse = m_y - sprite.rect.y;
                        activeSprite = &sprite;
                        propertyPanel.visible = true;
                        panelSelectedIndex = i;
                        break;
                    }
                }
            }
            if(!click_was_handled){
                activeSprite = NULL;
                panelSelectedIndex = -1;
                propertyPanel.visible = false;
                active_editing_block = nullptr;
            }
        }
    }
}

void engineUpdate(Engine& engine) {

    if(!engine.is_running_scripts){
        return;
    }

    for(auto& sprite : sprites){
        executeScriptsForSprite(&sprite);
    }
}


void initSprites(SDL_Renderer *renderer) {
    sprites.resize(3); // should be removed.    JUST FOR TEST
    for (int i = 0; i < sprites.size(); i++) {
        initSprite(sprites[i], &stage);
        sprites[i].rect.x += 50 * i;
        loadSpriteTexture(renderer, sprites[i], ASSETS_PATH + "test.bmp");
    }
    if (!sprites.empty()) {

        std::vector<Block> test_script;


        test_script.push_back({BlockType::ON_FLAG_CLICKED, {}, ""});
        test_script.push_back({BlockType::MOVE, {25}, ""});
        test_script.push_back({BlockType::SAY, {}, "Hello World!"});
        test_script.push_back({BlockType::TURN_RIGHT, {90}, ""});

        sprites[0].scripts.push_back(test_script);
    }
}

void drawSprites(SDL_Renderer *renderer) {

    for (int i = 0; i < sprites.size(); i++) {
        Sprite &sprite = sprites[i];
        if (sprite.show) {
            drawSprite(renderer, &sprite);
        }
    }
}

void drawSpritePanels(SDL_Renderer *renderer) {
    drawSpritePanelBase(renderer, &spritePanel);
    for (int i = 0; i < sprites.size(); i++) {
        Sprite &sprite = sprites[i];
        drawSpritePanelItem(renderer, &spritePanel, &sprite, i, panelSelectedIndex);
    }
}

//this function will be called in the main file before the while loop
void initBase(SDL_Renderer *renderer) {
    font = loadFont();
    initStage(&stage);
    initSprites(renderer);
    initSpritePanel(&spritePanel, renderer);
    initTopBar(&topBar);
    initPropertyPanel(renderer, &propertyPanel, windowConfig.width, windowConfig.height);
    initLibraryPanel(renderer, &libraryPanel);
    initScriptArea(&scriptArea);
    initBlockPalette(&blockPalette);
    initControlPanel(&controlPanel,renderer);
}

void engineDraw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderClear(renderer);

    drawBlockPalette(renderer, &blockPalette, font);
    drawScriptArea(renderer, &scriptArea, activeSprite, font);
    drawStage(renderer, &stage);
    drawSprites(renderer);
    drawTopBar(renderer, &topBar, font);
    drawControlPanel(renderer,&controlPanel);
    if (showSpritePanel) {
        drawSpritePanels(renderer);
    }
    if (activeSprite != nullptr) {
        drawPropertyPanel(renderer, &propertyPanel, activeSprite, font);
    }
    if (showLibraryPanel) {
        drawLibraryPanel(renderer, &libraryPanel);
    }
    if (dragged_block) {
        drawBlock(renderer, dragged_block, font);
    }
    SDL_RenderPresent(renderer);
}

void executeScriptsForSprite(Sprite* sprite) {

}