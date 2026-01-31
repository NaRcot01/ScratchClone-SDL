#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "core/sdl_init.h"
#include "core/engine.h"


int W=800,H=800;

int main(int argc, char* argv[]){
    SDL_Window* window;
    SDL_Renderer* renderer;
    if(!initSDL(window,renderer,W,H)){
        return 1;
    }

    Engine engine;
    engineInit(engine);

    while(engine.running){
        engineHandleEvents(engine);
        engineUpdate();
        engineDraw(renderer);
    }
    quitSDL(window,renderer);
    return 0;
}
