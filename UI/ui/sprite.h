// UI/sprite.h
// Unified sprite Header


#pragma once
#include <string>
#include <vector>
#include <map>
#include <SDL2/SDL.h>
#include "BLOCK.h"
#include "stage.h"


struct Script;

struct UnifiedSprite {

//visual properties

SDL_Rect rect;		// integer position for rendering
SDL_Texture* texture;
std::string name;
bool selected;
bool dragging;
int diff_x_mouse;
int diff_y_mouse;
int rotation;		// integer rotaion for rendering
int size;		// 100 percentage (normal)

// logical properties

//precise parameters
double exact_x;
double exact_y;
double direction;

bool isVisible;
int costumeIndex;
std::map<std::string, double> variables;

std::vector<Script> scripts;
};

//using "Sprite" as an alias
typedef UnifiedSprite Sprite;

//functions

void initSprite(Sprite& sprite, const Stage* stage);
void drawSprite(SDL_Renderer* renderer, const Sprite* sprite);
void spriteValidate(Sprite* sprite, const Stage* stage);
bool isSpriteClicked(int x, int y, const Sprite* sprite);
void moveSprite(Sprite* sprite, int x, int y, const Stage* stage, bool differential=false);
bool loadSpriteTexture(SDL_Renderer* renderer, Sprite& sprite, std::string path);
void updateSpriteSize(Sprite* sprite);

//new logic funtion
void updateSpriteLogic(Sprite* sprite, double deltaTime);