//
// Created by jack on 1/31/2026.
//

#include "engine.h"
#include "config.h"
#include "../ui/stage.h"
SDL_Event event;


Stage stage ;

void engineInit(Engine& engine){
    engine.running = true;
}

void engineHandleEvents(Engine& engine){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            engine.running = false;
        }
        if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED){
             int W = event.window.data1;
             int H = event.window.data2;

             windowConfig.height = H;
             windowConfig.width = W;

            resizeStage(&stage);
        }
    }
}

void engineUpdate(){

}


void engineDraw(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer,220,220,220,255);
    SDL_RenderClear(renderer);

    initStage(&stage);
    drawStage(renderer,&stage);

    SDL_RenderPresent(renderer);
}