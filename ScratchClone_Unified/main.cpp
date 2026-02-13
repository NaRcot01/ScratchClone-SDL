#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "core/config.h"
#include "core/sdl_init.h"
#include "core/engine.h"


int main(int argc, char* argv[]){
    SDL_Window* window;
    SDL_Renderer* renderer;

    if(!initSDL(window,renderer)){
        return 1;
    }



    Engine engine;
    SDL_Event event;
    engineInit(engine, event);
    initBase(engine, renderer);


    while(engine.running){
        engineHandleEvents(engine, event);
        engineUpdate(engine);
        engineDraw(engine, renderer);
    }
    quitSDL(window,renderer);
    return 0;
}
