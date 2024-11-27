#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <utility>

#include "config.h"
#include "Game/Walls.h"
#include "Game/Grid.h"

Walls::Walls()
{
    init();
}

Walls::~Walls()
{}

void Walls::init()
{
    populateWalls();
}

void Walls::update()
{}

void Walls::render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, c_wallColor.r, c_wallColor.g, c_wallColor.b, c_wallColor.a);       
                    
    for(std::pair<int, int> wall : walls){
        SDL_Rect destR = {wall.first*c_CELLSIZE + c_BORDERTHICKNESS, 
                          wall.second*c_CELLSIZE + c_BORDERTHICKNESS, 
                          c_CELLSIZE, c_CELLSIZE};
        SDL_RenderFillRect(renderer, &destR);
    }    
}

void Walls::populateGrid(Grid *grid)
{
    for(std::pair<int, int> wall : walls){
        grid->updateGrid(wall.first, wall.second, Grid::GridElements::wall);
    }
}

void Walls::populateWalls()
{   
    int count = 0;
    // General population of wall, only outer perimeter.
    for(int row = 0; row < Grid::rows; ++row){
        for(int col = 0; col < Grid::cols; ++col){
            if(row == 0 || row == Grid::rows - 1 || col == 0 || col == Grid::cols - 1){
                walls[count++] = {col, row};
            }
        }
    }
    wallNo = count;
}
