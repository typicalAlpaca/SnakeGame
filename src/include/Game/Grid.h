#ifndef GRID_H
#define GRID_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "config.h"

// Forward declaration of Snake class to remedy circular dependency.
class Snake;


class Grid{

public:
    enum GridElements {
        bodyNE, bodyNW, bodyNS, bodySE, bodySW, bodyEW, // Snake body parts
        headN, headS, headW, headE,                                 // Snake head parts
        tailN, tailS, tailW, tailE,                                 // Snake tail parts
        empty, wall, food
    };

public:
    Grid();
    ~Grid();

    void init();
    void update(Snake *snake);
    void render(SDL_Renderer *renderer, Snake *snake);
    
    void clearGrid();
    bool updateGrid(int x, int y, GridElements element);

public:
    const int winW = c_WINDOWWIDTH;
    const int winH = c_WINDOWHEIGHT;
    const int cellSize = c_CELLSIZE;
    const int borderWidth = c_BORDERTHICKNESS;
    static const int rows = c_WINDOWHEIGHT / c_CELLSIZE;
    static const int cols = c_WINDOWWIDTH / c_CELLSIZE;
    
private:
    int grid[rows][cols];
};

#endif // GRID_H
