// Created by jack on 1/31/2026.

#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
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
#include "block_executor.h"
#include "../ui/background_panel.h"
#include "../ui/backdrop_porperty_panel.h"
#include "project_io.h"

// ================
// Global Variables
// ================
SDL_Event event;
Stage stage;
std::vector<Sprite> sprites;
SpritePanel spritePanel;
Sprite *activeSprite = NULL;
int panelSelectedIndex = -1;
TopBar topBar;
TTF_Font *font;


PropertyPanel propertyPanel;
LibraryPanel libraryPanel;
ScriptArea scriptArea;
BlockPalette blockPalette;
ControlPanel controlPanel;
BackgroundPanel backgroundPanel;
BackdropPropertyPanel backdropPropertyPanel;


bool showSpritePanel = false;
bool showBGPanel = false;
bool showLibraryPanel = false;
bool showBackdropLibrary = false;


std::vector<ScriptState> running_scripts;
std::vector<std::string> script_logs;
unsigned long long int last_frame_time = 0;


Block* active_editing_block = nullptr;
int active_editing_param_index = -1;
std::string original_value_on_edit;

struct DragState {
    bool is_dragging = false;
    std::vector<Block> dragged_script;
    int source_sprite_index = -1;
    int source_script_index = -1;
    int offset_x = 0;
    int offset_y = 0;
    bool show_snap_preview = false;
    SDL_Rect snap_preview_rect;
};
DragState drag_state;


// Helper to check for horizontal alignment of blocks
bool areHorizontallyAligned(const SDL_Rect& r1, const SDL_Rect& r2) {
    return (r1.x < r2.x + r2.w && r1.x + r1.w > r2.x);
}


// ===============
// Event Handling
// ===============

void engineInit(Engine &engine) {
    engine.running = true;
}


void resetProject(SDL_Renderer* renderer, Engine &engine) {
    // log : initializing new project

    for (auto& sprite : sprites) { if(sprite.texture) SDL_DestroyTexture(sprite.texture); }
    for (auto& backdrop : stage.backgrounds) { if(backdrop.texture) SDL_DestroyTexture(backdrop.texture); }

    sprites.clear();
    stage.backgrounds.clear();
    running_scripts.clear();

    activeSprite = NULL;
    panelSelectedIndex = -1;
    stage.active_background_index = -1;

    propertyPanel.visible = false;
    backdropPropertyPanel.is_visible = false;
    engine.is_running_scripts = false;

    initSprite(sprites[0], &stage);

    sprites.clear();
    Sprite default_sprite;
    initSprite(default_sprite, &stage);
    loadSpriteTexture(renderer, default_sprite, ASSETS_PATH + "test.bmp");
    default_sprite.costume_path = "test.bmp";
    sprites.push_back(default_sprite);

    activeSprite = &sprites[0];
    panelSelectedIndex = 0;
    propertyPanel.visible = true;
}

void handleKeyDown(SDL_Event& event) {
    SDL_Keycode key = event.key.keysym.sym;

    //  handle block parameter editing
    if (active_editing_block) {
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
        }
        ParamType current_param_type = ParamType::NUMERIC;
        if (block_styles.count(active_editing_block->type)) {
            const auto &param_types = block_styles[active_editing_block->type].param_types;
            if (active_editing_param_index < param_types.size()) {
                current_param_type = param_types[active_editing_param_index];
            }
        }

        if (current_param_type == ParamType::NUMERIC) {
            if (key == SDLK_BACKSPACE && active_editing_block->parameters[active_editing_param_index] != 0) {
                active_editing_block->parameters[active_editing_param_index] =
                        (int) active_editing_block->parameters[active_editing_param_index] / 10;
            } else if (key >= SDLK_0 && key <= SDLK_9) {
                int entered_digit = key - SDLK_0;
                active_editing_block->parameters[active_editing_param_index] =
                        active_editing_block->parameters[active_editing_param_index] * 10 + entered_digit;
            }
        } else if (current_param_type == ParamType::STRING) {
            if (key == SDLK_BACKSPACE && !active_editing_block->textParam.empty()) {
                active_editing_block->textParam.pop_back();
            } else if ((key >= SDLK_a && key <= SDLK_z) || (key >= SDLK_0 && key <= SDLK_9) ||
                       key == SDLK_SPACE) {
                char typed_char = (char) key;
                if (SDL_GetModState() & KMOD_SHIFT) {
                    if (typed_char >= 'a' && typed_char <= 'z') typed_char -= 32;

                }
                active_editing_block->textParam += typed_char;
            }
        }

        return;
    }
    // while IF block is selected,  E btn is pressed ---> else block appear
    if (key == SDLK_e && activeSprite && !active_editing_block) {
        int m_x, m_y;
        SDL_GetMouseState(&m_x, &m_y);
        SDL_Point mouse_point = {m_x, m_y};

        for (auto& script : activeSprite->scripts) {
            for (int i = 0; i < script.size(); ++i) {
                // اگر روی یک بلوک IF کلیک شده
                if (script[i].type == BlockType::IF && SDL_PointInRect(&mouse_point, &script[i].rect)) {

                    // چک کن که آیا این IF از قبل ELSE دارد یا نه
                    int end_if_index = script[i].jumpToIndex;
                    if (end_if_index < script.size() && script[end_if_index].type == BlockType::ELSE) {
                        std::cout << "This IF block already has an ELSE." << std::endl;
                        return;
                    }

                    // --- منطق افزودن ELSE ---
                    Block else_block = {BlockType::ELSE, {}, ""};

                    // بلوک ELSE را بعد از محتوای داخلی IF، وارد کن
                    script.insert(script.begin() + end_if_index, else_block);

                    // حالا preprocessScript را دوباره فراخوانی کن تا تمام jumpToIndex ها آپدیت شوند
                    preprocessScript(script);

                    std::cout << "ELSE block added." << std::endl;
                    return; // کار تمام شد
                }
            }
        }
    }

    // handle backdrop name editing
    if (backdropPropertyPanel.is_editing_name && stage.active_background_index != -1) {
        Backdrop &active_backdrop = stage.backgrounds[stage.active_background_index];
        SDL_Keycode key = event.key.keysym.sym;

        if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
            backdropPropertyPanel.is_editing_name = false;
        } else if (key == SDLK_BACKSPACE && !active_backdrop.name.empty()) {
            active_backdrop.name.pop_back();
        } else if ((key >= SDLK_a && key <= SDLK_z) || (key >= SDLK_0 && key <= SDLK_9) || key == SDLK_SPACE) {
            active_backdrop.name += (char) key;
        }
        return;
    }

    // handle sprite property panel editing
    if (propertyPanel.visible && activeSprite) {
        for (auto &row1: propertyPanel.rows) {
            if (key == SDLK_BACKSPACE) {
                for (auto& row : propertyPanel.rows) {
                    if (row.active && !row.value.empty()) {
                        row.value.pop_back();
                        applyPropertyToSprite(row, activeSprite, &stage);
                        return;
                    }
                }
            }
            else if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
                for (auto& row : propertyPanel.rows) {
                    if (row.active) {
                        applyPropertyToSprite(row, activeSprite, &stage);
                        spriteValidate(activeSprite, &stage);
                        row.active = false;
                    }
                }
                return;
            }
            if (!row1.active) { continue; }

            char c = event.key.keysym.sym;

            if (row1.type != "PROPERTY_NAME") {
                if (c >= SDLK_0 && c <= SDLK_9) {
                    row1.value += char('0' + c - SDLK_0);
                }
            } else {
                if ((c >= SDLK_a && c <= SDLK_z) || (c >= SDLK_0 && c <= SDLK_9) || (c == SDLK_UNDERSCORE)) {
                    row1.value += char('a' + c - SDLK_a);
                }
            }
            applyPropertyToSprite(row1, activeSprite, &stage);
        }
    }
}

void handleMouseUp(SDL_Event& event) {
    // handle sprite frag release
    if (activeSprite && activeSprite->dragging) {
        activeSprite->dragging = false;
        activeSprite->x = activeSprite->rect.x;
        activeSprite->y = activeSprite->rect.y;
    }

    // handle block/script drag release
    if (drag_state.is_dragging) {
        SDL_Point mouse_point = {event.button.x, event.button.y};
        bool drop_handled = false;

        if (activeSprite && SDL_PointInRect(&mouse_point, &scriptArea.rect)) {

            if (drag_state.show_snap_preview) {
                int dx = drag_state.snap_preview_rect.x - drag_state.dragged_script.front().rect.x;
                int dy = drag_state.snap_preview_rect.y - drag_state.dragged_script.front().rect.y;
                for (auto& block : drag_state.dragged_script) {
                    block.rect.x += dx;
                    block.rect.y += dy;
                }
            }

            bool merged = false;
            for (int i = 0; i < activeSprite->scripts.size(); ++i) {
                auto& target_script = activeSprite->scripts[i];
                if (target_script.empty()) continue;

                for (int j = 0; j < target_script.size(); ++j) {
                    Block& target_block = target_script[j];
                    if (target_block.type == BlockType::REPEAT || target_block.type == BlockType::FOREVER || target_block.type == BlockType::IF  || target_block.type == BlockType::ELSE) {
                        SDL_Rect inner_snap_zone = {target_block.rect.x + 20, target_block.rect.y + 40, target_block.rect.w - 20, target_block.rect.h - 50};
                        if (SDL_PointInRect(&mouse_point, &inner_snap_zone)) {

                            target_script.insert(
                                    target_script.begin() + j + 1,
                                    drag_state.dragged_script.begin(),
                                    drag_state.dragged_script.end()
                            );

                            preprocessScript(target_script);
                            merged = true;
                            break;
                        }
                    }
                }

                Block& dragged_first = drag_state.dragged_script.front();
                Block& dragged_last = drag_state.dragged_script.back();
                Block& target_first = target_script.front();
                Block& target_last = target_script.back();

                if (abs((target_last.rect.y + target_last.rect.h) - dragged_first.rect.y) < 10) {
                    target_script.insert(target_script.end(), drag_state.dragged_script.begin(), drag_state.dragged_script.end());
                    preprocessScript(target_script);
                    merged = true;
                    break;
                }

                if (abs((dragged_last.rect.y + dragged_last.rect.h) - target_first.rect.y) < 10) {
                    drag_state.dragged_script.insert(drag_state.dragged_script.end(), target_script.begin(), target_script.end());
                    preprocessScript(target_script);
                    target_script = drag_state.dragged_script;
                    merged = true;
                    break;
                }
            }

            if (!merged) {
                preprocessScript(drag_state.dragged_script);
                activeSprite->scripts.push_back(drag_state.dragged_script);
            }

            drop_handled = true;
        }
        else if (SDL_PointInRect(&mouse_point, &blockPalette.block_panel_rect)) {
            // log : script deleted
            drop_handled = true;
        }
        else{
            if (activeSprite && drag_state.source_sprite_index != -1 && drag_state.source_sprite_index == panelSelectedIndex) {
                activeSprite->scripts.insert(activeSprite->scripts.begin() + drag_state.source_script_index, drag_state.dragged_script);
            }
        }

        drag_state.is_dragging = false;
        drag_state.dragged_script.clear();
        drag_state.source_sprite_index = -1;
        drag_state.source_script_index = -1;
    }

}

void handleMouseMotion(SDL_Event& event) {
    //  handle sprite frag motion
    if (activeSprite && activeSprite->dragging) {
        moveSprite(activeSprite, event.motion.x - activeSprite->diff_x_mouse, event.motion.y - activeSprite->diff_y_mouse, &stage);
    }

    // handle block/script drag motion
    if (drag_state.is_dragging) {
        int total_height = 0;
        for(const auto& b : drag_state.dragged_script) {
            total_height += b.rect.h + BLOCK_SPACING;
        }
        total_height -= BLOCK_SPACING;

        int new_x = event.motion.x - drag_state.offset_x;
        int new_y = event.motion.y - drag_state.offset_y;

        int dx = new_x - drag_state.dragged_script.front().rect.x;
        int dy = new_y - drag_state.dragged_script.front().rect.y;

        for (auto& block : drag_state.dragged_script) {
            block.rect.x += dx;
            block.rect.y += dy;
        }

        drag_state.show_snap_preview = false;

        if (activeSprite) {
            Block &dragged_first_block = drag_state.dragged_script.front();
            Block &dragged_last_block = drag_state.dragged_script.back();

            for (const auto &script: activeSprite->scripts) {
                if (script.empty()) continue;

                for (const auto& target_block : script) {
                    if (target_block.type == BlockType::REPEAT || target_block.type == BlockType::FOREVER || target_block.type == BlockType::IF) {
                        SDL_Rect inner_snap_zone = {
                                target_block.rect.x + 20,
                                target_block.rect.y + 40,
                                target_block.rect.w - 20,
                                target_block.rect.h - 50
                        };
                        SDL_Point p = {event.motion.x ,event.motion.y};
                        if (SDL_PointInRect(&p, &inner_snap_zone)) {
                            drag_state.show_snap_preview = true;
                            drag_state.snap_preview_rect = {
                                    target_block.rect.x + 20,
                                    target_block.rect.y + 40,
                                    drag_state.dragged_script.front().rect.w,
                            };
                            break;
                        }
                    }
                }

                const Block &target_last_block = script.back();
                SDL_Rect snap_zone_bottom = {target_last_block.rect.x,
                                             target_last_block.rect.y + target_last_block.rect.h + BLOCK_SPACING,
                                             target_last_block.rect.w, total_height};
                if (SDL_HasIntersection(&dragged_first_block.rect, &snap_zone_bottom)) {
                    drag_state.show_snap_preview = true;
                    drag_state.snap_preview_rect = {
                            target_last_block.rect.x,
                            target_last_block.rect.y + target_last_block.rect.h + 1,
                            dragged_first_block.rect.w,
                            (int) drag_state.dragged_script.size() * 45
                    };
                    break;
                }

                const Block &target_first_block = script.front();
                SDL_Rect snap_zone_top = {target_first_block.rect.x, target_first_block.rect.y - total_height - BLOCK_SPACING,
                                          target_first_block.rect.w, total_height};
                if (SDL_HasIntersection(&dragged_last_block.rect, &snap_zone_top)) {
                    drag_state.show_snap_preview = true;
                    drag_state.snap_preview_rect = {
                            target_first_block.rect.x,
                            target_first_block.rect.y - (int) drag_state.dragged_script.size() * 45,
                            dragged_first_block.rect.w,
                            (int) drag_state.dragged_script.size() * 45
                    };
                    break;
                }
            }
        }
    }
}

void handleMouseDown(SDL_Event& event, Engine& engine, SDL_Renderer* renderer) {
    int m_x = event.button.x;
    int m_y = event.button.y;
    SDL_Point mouse_point = {m_x, m_y};

    // library panels
    if (showLibraryPanel || showBackdropLibrary) {
        if (SDL_PointInRect(&mouse_point, &libraryPanel.closeBtnRect)) {
            showLibraryPanel = false;
            showBackdropLibrary = false;
            return;
        }

        for (size_t i = 0; i < libraryPanel.itemRects.size(); ++i) {
            if (SDL_PointInRect(&mouse_point, &libraryPanel.itemRects[i])) {
                const std::string &path = libraryPanel.itemPaths[i];

                if (showBackdropLibrary) {
                    addNewBackgroundFromFile(renderer, &stage, path.c_str());
                } else {
                    addNewSpriteFromFile(renderer, path.c_str(), stage, sprites);
                }

                showLibraryPanel = false;
                showBackdropLibrary = false;
                return;
            }
        }
    }

    // control buttons
    if (SDL_PointInRect(&mouse_point, &controlPanel.green_flag_rect)) {
        engine.is_running_scripts = true;
        // log : green flag clicked. starting scripts
        running_scripts.clear();
        for (int i = 0; i < sprites.size(); i++) {
            sprites[i].x = sprites[i].rect.x;
            sprites[i].y = sprites[i].rect.y;
            sprites[i].direction = sprites[i].rotation;
            for (int j = 0; j < sprites[i].scripts.size(); j++) {
                auto &script_blocks = sprites[i].scripts[j];
                if (!script_blocks.empty() && script_blocks.front().type == BlockType::ON_FLAG_CLICKED) {
                    preprocessScript(script_blocks);
                    running_scripts.push_back({i, j});

                }
            }
        }
        last_frame_time = SDL_GetTicks();
        return;
    }
    if (SDL_PointInRect(&mouse_point, &controlPanel.stop_button_rect)) {
        engine.is_running_scripts = false;
        // log : stop button clicked! stoping all scripts
         return;
    }

    // Topbar buttons
    for (int i = 0; i < topBar.buttonCount; i++) {
        TopBarButton &btn = topBar.buttons[i];
        if (btn.isClicked(m_x, m_y)) {
            if (btn.type == BTN_SPRITE_PANEL) {
                showSpritePanel = !showSpritePanel;
                showBGPanel = false;
            } else if (btn.type == BTN_STAGE_PANEL) {
                showBGPanel = !showBGPanel;
                showSpritePanel = false;
            }
            else if (btn.type == BTN_SAVE_PROJECT) {
                const char* filterPatterns[1] = {"*.json"};
                const char* path = tinyfd_saveFileDialog(
                        "Save Project",
                        "my_project.json",
                        1,
                        filterPatterns,
                        "JSON Project File"
                );
                if (path) {
                    if (saveProject(path)) {
                        // log : successfully the project has been saved
                    } else {
                        // log : saving project encounter error !
                    }
                }
            }
            else if (btn.type == BTN_LOAD_PROJECT) {
                const char* filterPatterns[1] = {"*.json"};
                const char* path = tinyfd_openFileDialog(
                        "Load Project",
                        "",
                        1,
                        filterPatterns,
                        "JSON Project File",
                        0
                );
                if (path) {

                    engine.is_running_scripts = false;
                    running_scripts.clear();

                    if (loadProject(path, renderer)) {
                        // log : successfully the project has been loaded
                    } else {
                        // log : loading project encounter error !
                    }
                }
            }
            else if (btn.type == BTN_NEW_PROJECT) {
                int choice = tinyfd_messageBox(
                        "New Project",
                        "Do you want to save the current project before creating a new one?",
                        "yesnocancel",
                        "question",
                        1
                );

                // 1 = Yes, 2 = No, 0 = Cancel
                bool proceed = false;
                if (choice == 1) { // Yes
                    const char* filterPatterns[1] = {"*.json"};
                    const char* save_path = tinyfd_saveFileDialog("Save Project", "my_project.json", 1, filterPatterns, "JSON Project File");
                    if (save_path && saveProject(save_path)) {
                        proceed = true;
                    }
                }
                else if (choice == 2) { // No
                    proceed = true;
                }
                if (proceed) {
                    resetProject(renderer,engine);
                }
            }
            return;
        }
    }

    // start a drag operation
    // from block palette
    if (SDL_PointInRect(&mouse_point, &blockPalette.block_panel_rect)) {
        for (auto& template_block : blockPalette.template_blocks) {
            if (SDL_PointInRect(&mouse_point, &template_block.rect)) {

                drag_state.is_dragging = true;

                drag_state.dragged_script.clear();
                drag_state.dragged_script.push_back(template_block);

                if (template_block.type == BlockType::REPEAT || template_block.type == BlockType::FOREVER) {
                    Block end_block = {BlockType::END_REPEAT, {}, ""};
                    drag_state.dragged_script.push_back(end_block);
                }
                else if (template_block.type == BlockType::IF) {
                    Block end_block = {BlockType::END_IF, {}, ""};
                    drag_state.dragged_script.push_back(end_block);
                }

                drag_state.source_sprite_index = -1;
                drag_state.source_script_index = -1;

                // the real position
                drag_state.offset_x = m_x - template_block.rect.x;
                drag_state.offset_y = m_y - template_block.rect.y;



                return;
            }
        }
    }

    // click in script area
    if (activeSprite && SDL_PointInRect(&mouse_point, &scriptArea.rect)) {
        for (auto& script : activeSprite->scripts) {
            for (auto& block : script) {
                if (!SDL_PointInRect(&mouse_point, &block.rect)) continue;

                for (size_t i = 0; i < block.param_rects.size(); ++i) {
                    if (SDL_PointInRect(&mouse_point, &block.param_rects[i])) {
                        active_editing_block = &block;
                        active_editing_param_index = i;
                        return;
                    }
                }
            }
        }
        for (int i = activeSprite->scripts.size() - 1; i >= 0; --i) {
            for (int j = activeSprite->scripts[i].size() - 1; j >= 0; --j) {

                Block& current_block = activeSprite->scripts[i][j];

                if (SDL_PointInRect(&mouse_point, &current_block.rect)) {

                    drag_state.is_dragging = true;

                    drag_state.dragged_script.assign(
                            activeSprite->scripts[i].begin() + j,
                            activeSprite->scripts[i].end()
                    );

                    activeSprite->scripts[i].resize(j);

                    if (activeSprite->scripts[i].empty()) {
                        activeSprite->scripts.erase(activeSprite->scripts.begin() + i);
                    }

                    drag_state.source_sprite_index = panelSelectedIndex;
                    drag_state.source_script_index = i;
                    drag_state.offset_x = m_x - current_block.rect.x;
                    drag_state.offset_y = m_y - current_block.rect.y;



                    return;
                }
            }
        }
    }


    // block parameter editing
    if (activeSprite) {
        for (auto &script: activeSprite->scripts) {
            for (auto &block: script) {

                if (!SDL_PointInRect(&mouse_point, &block.rect)) continue;

                for (size_t i = 0; i < block.param_rects.size(); ++i) {
                    if (SDL_PointInRect(&mouse_point, &block.param_rects[i])) {

                        active_editing_block = &block;
                        active_editing_param_index = i;

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
                        break;
                    }
                }
            }
        }
    }
    // backdrop property panel (Name, Delete)
    if (showBGPanel && backdropPropertyPanel.is_visible) {
        // edit name
        if (SDL_PointInRect(&mouse_point, &backdropPropertyPanel.name_input_rect)) {
            backdropPropertyPanel.is_editing_name = true;


            if (stage.active_background_index != -1) {
                stage.backgrounds[stage.active_background_index].name = "";
            }

            return;
        }

        // delete backdrop
        if (SDL_PointInRect(&mouse_point, &backdropPropertyPanel.delete_button_rect)) {
            if (stage.active_background_index != -1) {
                int index_to_delete = stage.active_background_index;

                if (index_to_delete < stage.backgrounds.size() && stage.backgrounds[index_to_delete].texture) {
                    SDL_DestroyTexture(stage.backgrounds[index_to_delete].texture);
                }

                if (index_to_delete < stage.backgrounds.size()) {
                    stage.backgrounds.erase(stage.backgrounds.begin() + index_to_delete);
                }

                if (!stage.backgrounds.empty()) {
                    stage.active_background_index = (index_to_delete < stage.backgrounds.size()) ? index_to_delete : stage.backgrounds.size() - 1;
                } else {
                    stage.active_background_index = -1;
                    backdropPropertyPanel.is_visible = false;
                }
            }

            return;
        }


        if (SDL_PointInRect(&mouse_point, &backdropPropertyPanel.rect)) {
            backdropPropertyPanel.is_editing_name = false;
            return;
        }
    }

    // sprite property panel
    if (propertyPanel.visible && activeSprite) {
        // click is handled by another function within property_panel.cpp
        if (handlePropertyPanelClicked(&propertyPanel, activeSprite, m_x, m_y)) {
            return;
        }
    }

    if (showBGPanel) {
        // upload btn
        if (SDL_PointInRect(&mouse_point, &backgroundPanel.upload_button_rect)) {
            const char* filterPatterns[3] = {"*.png", "*.jpg", "*.bmp"};
            const char* filePath = tinyfd_openFileDialog(
                    "Choose a Backdrop Image",
                    "", 3, filterPatterns, "Image Files", 0
            );
            addNewBackgroundFromFile(renderer, &stage, filePath);
            return;
        }

       // library btn
        if (SDL_PointInRect(&mouse_point, &backgroundPanel.library_button_rect)) {
            showBackdropLibrary = true;
            cleanupLibraryPanel(&libraryPanel);
            initLibraryPanel(&libraryPanel, renderer, backdrop_library_files);
            return;
        }
        // random btn
        if (SDL_PointInRect(&mouse_point, &backgroundPanel.random_button_rect)) {
            if (!backdrop_library_files.empty()) {

                unsigned seed = time(nullptr);
                std::mt19937 gen(seed);
                std::uniform_int_distribution<> distrib(0, backdrop_library_files.size() - 1);
                int random_idx = distrib(gen);

                std::string random_path = ASSETS_PATH + backdrop_library_files[random_idx];
                addNewBackgroundFromFile(renderer, &stage, random_path.c_str());
            }
            return;
        }

        // change active backdrop
        int current_y = backgroundPanel.upload_button_rect.y + backgroundPanel.upload_button_rect.h + 10;
        for (int i = 0; i < stage.backgrounds.size(); ++i) {
            SDL_Rect item_rect = {
                    backgroundPanel.rect.x + 10,
                    current_y,
                    backgroundPanel.rect.w - 20,
                    100
            };

            if (SDL_PointInRect(&mouse_point, &item_rect)) {
                stage.active_background_index = i;
                backdropPropertyPanel.is_visible = true;
                backdropPropertyPanel.is_editing_name = false;
                return;
            }
            current_y += item_rect.h + 10;
        }

        if (SDL_PointInRect(&mouse_point, &backgroundPanel.rect)) {
            return;
        }
    }
    // sprite panel
    if (showSpritePanel) {
        // upload btn
        if (SDL_PointInRect(&mouse_point, &spritePanel.buttonRects[0])) {
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
                addNewSpriteFromFile(renderer, filePath, stage, sprites);
            }
            return;
        }
        // library btn
        if (SDL_PointInRect(&mouse_point, &spritePanel.buttonRects[1])) {
            showLibraryPanel = true;
            cleanupLibraryPanel(&libraryPanel);
            initLibraryPanel(&libraryPanel, renderer, libraryFiles);
            return;
        }
        // random btn
        if (SDL_PointInRect(&mouse_point, &spritePanel.buttonRects[2])) {
            unsigned seed = time(nullptr);
            std::mt19937 gen(seed);

            std::uniform_int_distribution<> distrib(0, libraryFiles.size() - 1);
            int random_idx = distrib(gen);
            std::string random_path = ASSETS_PATH + libraryFiles[random_idx];
            addNewSpriteFromFile(renderer, random_path.c_str(), stage, sprites);
            return;
        }
        // paint btn. LATER

       // click on sprite item in sprite panel
        for (int i = 0; i < sprites.size(); i++) {
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

            // remove sprite
            if (SDL_PointInRect(&mouse_point, &deleteBtnRect)) {
                if (activeSprite == &sprites[i]) {
                    activeSprite = NULL;
                    panelSelectedIndex = -1;
                    propertyPanel.visible = false;
                }
                sprites.erase(sprites.begin() + i);
                return;
            }

            if (SDL_PointInRect(&mouse_point, &itemRect)) {
                activeSprite = &sprites[i];
                panelSelectedIndex = i;
                propertyPanel.visible = true;
                activeSprite->selected = true;
                return;
            }
        }

        if (SDL_PointInRect(&mouse_point, &spritePanel.rect)) {
            return;
        }
    }


// clicking sprites on stage
    for (int i = 0; i < sprites.size(); ++i) {
        if (sprites[i].show && isSpriteClicked(m_x, m_y, &sprites[i])) {

            // deselect all sprites
            for (int j = 0; j < sprites.size(); ++j) {
                if (i != j) {
                    sprites[j].selected = false;
                }
            }


            activeSprite = &sprites[i];
            activeSprite->selected = true;
            panelSelectedIndex = i;

            activeSprite->dragging = true;

            // for dragging sprites this is needed
            activeSprite->diff_x_mouse = m_x - activeSprite->rect.x;
            activeSprite->diff_y_mouse = m_y - activeSprite->rect.y;

            propertyPanel.visible = true;
            showBGPanel = false;

            active_editing_block = nullptr;
            backdropPropertyPanel.is_editing_name = false;

            // log : Sprite activeSprite->name selected from stage.


            return;
        }
    }


    // mouse is clicked on a empty space so nothing to do.
    activeSprite = NULL;
    panelSelectedIndex = -1;
    propertyPanel.visible = false;
    backdropPropertyPanel.is_visible = false;
    active_editing_block = nullptr;
}


// ================
// Main Event Loop
// ================
void engineHandleEvents(Engine &engine, SDL_Renderer *renderer) {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                engine.running = false;
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    windowConfig.width = event.window.data1;
                    windowConfig.height = event.window.data2;
                    resizeStage(&stage);
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                handleMouseDown(event, engine, renderer);
                break;

            case SDL_MOUSEBUTTONUP:
                handleMouseUp(event);
                break;

            case SDL_MOUSEMOTION:
                handleMouseMotion(event);
                break;

            case SDL_KEYDOWN:
                handleKeyDown(event);
                break;

            case SDL_MOUSEWHEEL:
                SDL_Point p = {event.wheel.x};
                if (SDL_PointInRect(&p, &scriptArea.rect)) {
                    scriptArea.scroll_offset_y -= event.wheel.y * 20;
                    if (scriptArea.scroll_offset_y < 0) scriptArea.scroll_offset_y = 0;
                }
                break;
        }
    }
}

// =============
// Update & Draw
// =============

void engineUpdate(Engine &engine) {

    if (!engine.is_running_scripts) {
        return;
    }
    unsigned long long int current_time = SDL_GetTicks();
    double deltaTime = (current_time - last_frame_time) / 1000.0;
    last_frame_time = current_time;
    script_logs.clear();
    for (auto &state: running_scripts) {
        if (state.isActive) {
            Sprite &current_sprite = sprites[state.sprite_index];
            updateScript(state, current_sprite, deltaTime, script_logs);
        }
    }

    running_scripts.erase(std::remove_if(running_scripts.begin(), running_scripts.end(), helperFunc_scriptRemover),
                          running_scripts.end());

    for (auto &sprite: sprites) {
        sprite.rect.x = sprite.x;
        sprite.rect.y = sprite.y;
        sprite.rotation = sprite.direction;

        updateSpriteSize(&sprite);
    }


}


void initSprites(SDL_Renderer *renderer) {
    sprites.resize(2); // should be removed.    JUST FOR TEST
    for (int i = 0; i < sprites.size(); i++) {
        initSprite(sprites[i], &stage);
        sprites[i].rect.x += 50 * i;
        loadSpriteTexture(renderer, sprites[i], ASSETS_PATH + "test.bmp");
    }
    if (!sprites.empty()) {
        std::vector<Block> test_script;

        Block b1 = {BlockType::ON_FLAG_CLICKED, {}, ""};
        b1.rect = {350, 100, 220, 40};
        test_script.push_back(b1);

        Block b2 = {BlockType::MOVE, {25}, ""};
        b2.rect = {350, b1.rect.y + b1.rect.h + BLOCK_SPACING, 220, 40};
        test_script.push_back(b2);

        Block b3 = {BlockType::TURN_RIGHT, {90}, ""};
        b3.rect = {350, b2.rect.y + b2.rect.h + BLOCK_SPACING, 220, 40};
        test_script.push_back(b3);


        sprites[0].scripts.push_back(test_script);
    }
}

void drawSprites(SDL_Renderer *renderer) {

    for (int i = sprites.size() - 1; i >= 0; i--) {
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
    initLibraryPanel(&libraryPanel, renderer, libraryFiles);
    initScriptArea(&scriptArea);
    initBlockPalette(&blockPalette);
    initControlPanel(&controlPanel, renderer);
    initBackgroundPanel(&backgroundPanel, renderer);
    initBackdropPropertyPanel(&backdropPropertyPanel, renderer);
}

void engineDraw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderClear(renderer);

    drawBlockPalette(renderer, &blockPalette, font);
    drawScriptArea(renderer, &scriptArea, activeSprite, font);
    drawStage(renderer, &stage);
    drawSprites(renderer);
    drawTopBar(renderer, &topBar, font);
    drawControlPanel(renderer, &controlPanel);
    if (showSpritePanel) {
        drawSpritePanels(renderer);
    } else if (showBGPanel) {
        drawBackgroundPanel(renderer, &backgroundPanel, &stage, font);
        if (stage.active_background_index != -1) {
            const auto &active_backdrop = stage.backgrounds[stage.active_background_index];
            drawBackdropPropertyPanel(renderer, &backdropPropertyPanel, active_backdrop.name, font);
        }
    }
    if (activeSprite != nullptr) {
        drawPropertyPanel(renderer, &propertyPanel, activeSprite, font);
    }
    if (showLibraryPanel) {
        drawLibraryPanel(renderer, &libraryPanel,font);
    }
    if (showBackdropLibrary) {
        drawLibraryPanel(renderer, &libraryPanel,font);
    }

    if (drag_state.is_dragging) {
        if (drag_state.show_snap_preview) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);
            SDL_RenderFillRect(renderer, &drag_state.snap_preview_rect);
        }

        for (auto& block : drag_state.dragged_script) {
            drawBlock(renderer, &block, font);
        }
    }

    SDL_RenderPresent(renderer);
}
