#ifndef GRID_H
#define GRID_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <utility>

#include "config.h"

// Forward declaration of Snake class to remedy circular dependency.
class Snake;
class Walls;
class Food;


class Grid{

public:
    enum GridElements {empty, wall, snake, food};

public:
    Grid();
    ~Grid();

    void init();
    void update(Snake *snake, Food *food);
    void render(SDL_Renderer *renderer);
    
    void clearGrid();
    void updateGrid(int x, int y, GridElements element);
    GridElements get(int x, int y);

public:
    static const int rows = c_WINDOWHEIGHT / c_CELLSIZE;
    static const int cols = c_WINDOWWIDTH / c_CELLSIZE;
    bool collide = false;
    std::pair<int, int> walls[rows*cols];
    int available = rows*cols;

private:
    GridElements grid[rows][cols];

};

#endif // GRID_H
