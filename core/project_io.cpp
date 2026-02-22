

#include "project_io.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "../ui/script_area.h"

extern std::vector<Sprite> sprites;
extern Stage stage;
extern std::string ASSETS_PATH;
extern void preprocessScript(std::vector<Block>& v);
extern void addNewBackgroundFromFile(SDL_Renderer* renderer, Stage* stage, const char* filePath);
extern bool loadSpriteTexture(SDL_Renderer *renderer, Sprite &sprite, std::string path);

static void saveSprite(std::ofstream& out, const Sprite& s) {
    out << "SPRITE\n";
    out << "name " << s.name << "\n";
    out << "costume_path " << s.costume_path << "\n";
    out << "x " << s.x << "\n";
    out << "y " << s.y << "\n";
    out << "direction " << s.direction << "\n";
    out << "size " << s.size << "\n";
    out << "visible " << (s.show ? 1 : 0) << "\n";
    out << "flip_mode " << static_cast<int>(s.flip_mode) << "\n";

    out << "SCRIPTS " << s.scripts.size() << "\n";
    for (const auto& script_blocks : s.scripts) {
        int script_x = script_blocks.empty() ? 0 : script_blocks.front().rect.x;
        int script_y = script_blocks.empty() ? 0 : script_blocks.front().rect.y;

        out << "SCRIPT_BLOCKS " << script_blocks.size() << " " << script_x << " " << script_y << "\n";
        for (const Block& b : script_blocks) {
            out << "BLOCK ";
            out << static_cast<int>(b.type) << " ";
            out << b.jumpToIndex << " ";
            out << b.parameters.size();
            for (double v : b.parameters) {
                out << " " << std::fixed << std::setprecision(6) << v;
            }
            out << " " << (b.textParam.empty() ? "-" : b.textParam) << "\n";
        }
    }
    out << "END_SPRITE\n";
}

static void saveStage(std::ofstream& out, const Stage& st) {
    out << "STAGE\n";
    out << "active_backdrop " << st.active_background_index << "\n";
    out << "BACKDROPS " << st.backgrounds.size() << "\n";
    for(const auto& backdrop : st.backgrounds) {
        out << "BACKDROP " << backdrop.name << " " << backdrop.file_path << "\n";
    }
    out << "END_STAGE\n";
}

bool saveProject(const std::string& path) {
    std::ofstream out(path);
    if (!out.is_open()) return false;

    out << "SCRATCH_CLONE_PROJECT_V1\n";

    saveStage(out, stage);

    out << "SPRITES " << sprites.size() << "\n";
    for (const Sprite& s : sprites) {
        saveSprite(out, s);
    }
    out << "END_PROJECT\n";
    return true;
}



static bool loadSprite(std::ifstream& in, Sprite& s, SDL_Renderer* renderer) {
    std::string token, line;
    std::string temp_costume_path;
    while (std::getline(in, line) && line != "END_SPRITE") {
        std::stringstream ss(line);
        ss >> token;

        if (token == "name") { ss >> s.name; }
        else if (token == "costume_path") {
            ss >> temp_costume_path;
        }
        else if (token == "x") { ss >> s.x; s.rect.x = s.x; }
        else if (token == "y") { ss >> s.y; s.rect.y = s.y; }
        else if (token == "direction") { ss >> s.direction; s.rotation = s.direction; }
        else if (token == "size") { ss >> s.size; updateSpriteSize(&s); }
        else if (token == "visible") { int v; ss >> v; s.show = (v != 0); }
        else if (token == "flip_mode") { int v; ss >> v; s.flip_mode = (SDL_RendererFlip)v; }
        else if (token == "SCRIPTS") {
            size_t scriptCount; ss >> scriptCount;
            s.scripts.assign(scriptCount, std::vector<Block>());
            for (size_t i = 0; i < scriptCount; ++i) {
                std::getline(in, line); std::stringstream script_ss(line);

                script_ss >> token;
                size_t blockCount;
                int script_x, script_y;
                script_ss >> blockCount >> script_x >> script_y;

                s.scripts[i].reserve(blockCount);
                for (size_t b = 0; b < blockCount; ++b) {
                    std::getline(in, line); std::stringstream block_ss(line);
                    block_ss >> token;
                    Block blk;
                    int typeInt; block_ss >> typeInt; blk.type = static_cast<BlockType>(typeInt);
                    block_ss >> blk.jumpToIndex;
                    size_t paramCount; block_ss >> paramCount;
                    blk.parameters.resize(paramCount);
                    for (size_t p = 0; p < paramCount; ++p) block_ss >> blk.parameters[p];
                    block_ss >> blk.textParam;
                    if(blk.textParam == "-") blk.textParam = "";
                    s.scripts[i].push_back(blk);
                }
                if (!s.scripts[i].empty()) {
                    s.scripts[i].front().rect.x = script_x;
                    s.scripts[i].front().rect.y = script_y;

                    preprocessScript(s.scripts[i]);
                    calculateLayout(s.scripts[i], 0, s.scripts[i].size(), script_x, script_y);
                }
            }
        }
    }
    if (!temp_costume_path.empty()) {
        s.costume_path = temp_costume_path;
        loadSpriteTexture(renderer, s, ASSETS_PATH + s.costume_path);
    }
    else {
        loadSpriteTexture(renderer, s, ASSETS_PATH + "test.bmp");
    }
    return true;
}

static bool loadStage(std::ifstream& in, Stage& st, SDL_Renderer* renderer) {
    std::string token, line;
    int saved_active_index = -1;

    while (std::getline(in, line) && line != "END_STAGE") {
        std::stringstream ss(line);
        ss >> token;

        if(token == "active_backdrop") { ss >> st.active_background_index; }
        else if(token == "BACKDROPS") {
            size_t count; ss >> count;
            st.backgrounds.clear();
            for(size_t i = 0; i < count; ++i) {
                std::getline(in, line);
                std::stringstream backdrop_ss(line);

                backdrop_ss >> token;

                std::string name, file_path;
                backdrop_ss >> name >> file_path;

                std::string full_path = ASSETS_PATH + file_path;
                SDL_Texture* tex = IMG_LoadTexture(renderer, full_path.c_str());
                if(tex) {
                    st.backgrounds.push_back({tex, name, file_path});
                } else {
                    // log :  "Failed to load backdrop: "  full_path
                }
            }
        }
    }
    in.clear();
    in.seekg(0);
    while(std::getline(in, line) && line != "END_STAGE") {
        std::stringstream ss(line);
        ss >> token;
        if(token == "active_backdrop") { ss >> st.active_background_index; break; }
    }
    return true;
}


bool loadProject(const std::string& path, SDL_Renderer* renderer) {
    std::ifstream in(path);
    if (!in.is_open()) return false;

    std::string header, line;
    std::getline(in, line);
    if (line != "SCRATCH_CLONE_PROJECT_V1") return false;

    for (auto& sprite : sprites) { if(sprite.texture) SDL_DestroyTexture(sprite.texture); }
    sprites.clear();
    for (auto& backdrop : stage.backgrounds) { if(backdrop.texture) SDL_DestroyTexture(backdrop.texture); }
    stage.backgrounds.clear();
    stage.active_background_index = -1;

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "STAGE") {
            loadStage(in, stage, renderer);
        }
        else if (token == "SPRITES") {
            size_t spriteCount; ss >> spriteCount;
            sprites.resize(spriteCount);
            for (size_t i = 0; i < spriteCount; ++i) {
                std::getline(in, line); // "SPRITE"
                initSprite(sprites[i], &stage);
                loadSprite(in, sprites[i],renderer);
            }
        }
        else if (token == "END_PROJECT") {
            break;
        }
    }
    return true;
}
