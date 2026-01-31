//
// Created by jack on 1/31/2026.
//

#include "engine.h"
#include "../ui/stage.h"
SDL_Event event;


Stage stage = {{300,50,550,400}};

void engineInit(Engine& engine){
    engine.running = true;
}

void engineHandleEvents(Engine& engine){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            engine.running = false;
        }
    }
}

void engineUpdate(){

}


void engineDraw(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer,220,220,220,255);
    SDL_RenderClear(renderer);

    drawStage(renderer,stage);

    SDL_RenderPresent(renderer);
}