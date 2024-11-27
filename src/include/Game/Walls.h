#ifndef WALLS_H
#define WALLS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <utility>

#include "config.h"
#include "Game/Grid.h"

class Walls{

public:
    Walls();
    ~Walls();

    void init();
    void update();
    void render(SDL_Renderer *renderer);
    
    void populateGrid(Grid *grid);
    void populateWalls();

public:
    std::pair<int, int> walls[Grid::cols*Grid::rows] = {};
    int wallNo = 0;

};

#endif // WALLS_H
