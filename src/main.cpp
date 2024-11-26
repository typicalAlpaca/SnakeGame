#include <SDL2/SDL.h>
#include <iostream>

#include "Game/Game.h"
#include "config.h"

Game *game;
const int FRAME_DELAY = 1000 / c_FPS;
Uint32 frameStart;
int frameTime;
int frames = 0;
int tickFPS = c_FPS / c_ticksPerSec;

int main(int argc, char* argv[])
{
    game = new Game();

    while(game->running()){
        frameStart = SDL_GetTicks();
        
        game->handleEvents();

        // Controls number of ticks per second using c_ticksPerSec
        if(frames%tickFPS == 0){
            game->update();
            game->render();
        }

        // Maintains FPS at c_FPS
        while(SDL_GetTicks() - frameStart < FRAME_DELAY)
        {
            game->handleEvents();
            SDL_Delay(10);    
        }
        
        frames = (frames + 1) % 59;
    }

    game->clean();
    return EXIT_SUCCESS;
}