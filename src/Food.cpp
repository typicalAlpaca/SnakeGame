#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <utility>
#include <random>

#include "Game/Food.h"
#include "Game/Grid.h"
#include "Game/Snake.h"
#include "Game/Walls.h"
#include "config.h"

Food::Food(SDL_Renderer *renderer)
{
    init(renderer);
}

Food::~Food()
{}

void Food::init(SDL_Renderer *renderer)
{
    initRandomEngine();

    SDL_Surface* tempSurface = IMG_Load(c_SNAKEDIR);
    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 255, 255, 255));
    foodTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    SDL_Log("Food initialized");
}

void Food::update(Grid *grid, Snake *snake)
{}

void Food::render(SDL_Renderer *renderer)
{
    SDL_Rect destR = {coord.first*c_CELLSIZE + c_BORDERTHICKNESS_LEFT, 
                      coord.second*c_CELLSIZE + c_BORDERTHICKNESS_TOP, 
                      c_CELLSIZE, c_CELLSIZE};
    SDL_RenderCopy(renderer, foodTexture, &foodRect, &destR);
}

void Food::getNewFood(Grid *grid, Snake *snake, Walls *walls)
{
    // Find how many empty cells there are in the grid first
    int empty = grid->available - snake->length - walls->wallNo;
    
    // Randomly select a slot index n
    std::uniform_int_distribution<int> rand(0, empty);
    int n = rand(engine);

    // Iterate through grid and use the nth empty slot
    for(int row = 0; row < Grid::rows; ++row){
        for(int col = 0; col < Grid::cols; ++col){
            if(grid->get(col, row) == Grid::GridElements::empty){
                if(n==0){
                    coord = {col, row};
                    return;
                } else{
                    n--;
                }
            }
        }
    }
    return;
}

void Food::populateGrid(Grid *grid)
{
    grid->updateGrid(coord.first, coord.second, Grid::GridElements::food);
}

void Food::initRandomEngine()
{
    std::random_device rd;
    engine = std::default_random_engine(rd());
}
