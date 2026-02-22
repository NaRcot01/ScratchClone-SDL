//
// Created by jack on 1/31/2026.
//
#include "sdl_init.h"
#include "config.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

bool initSDL(SDL_Window* &window, SDL_Renderer* &renderer){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        // log :
        std::cout << "sdl init failed"<<std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)){
        // log : sdl image can not be initialize.
        return false;
    }

    int mixFlags = MIX_INIT_MP3 | MIX_INIT_OGG;
    if (!(Mix_Init(mixFlags) & mixFlags)) {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not open audio! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Scratch",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,windowConfig.width,windowConfig.height,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(!window){
        // log :
        std::cout << "sdl create window failed"<<std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer){
        // log :
        std::cout << "sdl create renderer failed"<<std::endl;
        return false;
    }

    return true;
}


void quitSDL(SDL_Window* &window, SDL_Renderer* &renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}