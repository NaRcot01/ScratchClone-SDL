//
// Created by jack on 1/31/2026.
//
#include "sdl_init.h"
#include "config.h"

bool initSDL(SDL_Window* &window, SDL_Renderer* &renderer){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        // log :
        std::cout << "sdl init failed"<<std::endl;
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
    SDL_Quit();
}