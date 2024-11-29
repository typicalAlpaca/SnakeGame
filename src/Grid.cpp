#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Game/Grid.h"
#include "Game/Snake.h"
#include "Game/Walls.h"
#include "Game/Food.h"
#include "config.h"

Grid::Grid()
{
    init();
}

Grid::~Grid()
{}

void Grid::init()
{
    clearGrid();
    collide = false;
}

void Grid::update(Snake *snake, Food *food)
{
    clearGrid();
    snake->populateGrid(this);
    food->populateGrid(this);
}

void Grid::render(SDL_Renderer *renderer)
{
    // Render empty cells of the grid only
    for(int row = 0; row < rows; ++row){
        for(int col = 0; col < cols; ++col){
            SDL_Rect destR = {col*c_CELLSIZE + c_BORDERTHICKNESS_LEFT, row*c_CELLSIZE + c_BORDERTHICKNESS_TOP, 
                              c_CELLSIZE, c_CELLSIZE};
            if(grid[row][col] == Grid::GridElements::empty){
                SDL_SetRenderDrawColor(renderer, c_bgColor.r, c_bgColor.g, c_bgColor.b, c_bgColor.a);
                SDL_RenderFillRect(renderer, &destR);
            }
        }
    }
}

void Grid::clearGrid(){
    // Clear snake and food from grid, retain walls so only need to initialize once.
    for(int row = 0; row < rows; ++row){
        for(int col = 0; col < cols; ++col){
            if(grid[row][col] == Grid::GridElements::wall){
                continue;
            } else{
                grid[row][col] = Grid::GridElements::empty;
            }
        }
    }
}

void Grid::updateGrid(int x, int y, Grid::GridElements element)
{
    if(!(y < rows && x < cols))
    {
        SDL_Log("Trying to update grid out of bounds.");
    }

    if(grid[y][x] != Grid::GridElements::empty && element != Grid::GridElements::wall){
        collide = true;
    }

    grid[y][x] = element;
}

Grid::GridElements Grid::get(int x, int y)
{
    return grid[y][x];
}
