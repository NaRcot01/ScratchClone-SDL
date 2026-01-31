//
// Created by jack on 1/31/2026.
//
#include "sdl_init.h"


bool initSDL(SDL_Window* &window, SDL_Renderer* &renderer,int W, int H){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        // log :
        std::cout << "sdl init failed"<<std::endl;
        return false;
    }

    window = SDL_CreateWindow("Scratch",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,W,H,SDL_WINDOW_SHOWN);
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