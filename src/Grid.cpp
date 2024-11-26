#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Game/Grid.h"
#include "Game/Snake.h"
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

    // Set the snake in the center
    grid[rows/2][cols/2] = GridElements::headE;
    grid[rows/2][cols/2-1] = GridElements::tailE;
}

void Grid::update(Snake *snake)
{
    clearGrid();
}

void Grid::render(SDL_Renderer *renderer, Snake *snake)
{
    // Render each cell of the grid individually
    for(int row = 0; row < rows; ++row){
        for(int col = 0; col < cols; ++col){
            SDL_Rect destR = {col*cellSize + borderWidth, row*cellSize + borderWidth, cellSize, cellSize};
            switch(grid[row][col]){
                case GridElements::wall:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White       
                    SDL_RenderFillRect(renderer, &destR);
                    break;
                case GridElements::empty:
                    SDL_SetRenderDrawColor(renderer, c_bgColor.r, c_bgColor.g, c_bgColor.b, c_bgColor.a);
                    SDL_RenderFillRect(renderer, &destR);
                    break;
                case GridElements::food:
                    break;
                case GridElements::bodyEW:
                case GridElements::bodyNE:
                case GridElements::bodyNS:
                case GridElements::bodyNW:
                case GridElements::bodySE:
                case GridElements::bodySW:
                case GridElements::headE:
                case GridElements::headN:
                case GridElements::headS:
                case GridElements::headW:
                case GridElements::tailE:
                case GridElements::tailN:
                case GridElements::tailS:
                case GridElements::tailW:
                    SDL_RenderCopy(renderer, snake->snakeTexture, &snake->snakePartsRect[grid[row][col]], &destR);
                    break;
                default:
                    break;
            }
        }
    }
}

void Grid::clearGrid(){
    // Loop initialize the grid
    for(int row = 0; row < rows; ++row){
        for(int col = 0; col < cols; ++col){
            if(row == 0 || row == rows - 1 || col == 0 || col == cols - 1){
                grid[row][col] = GridElements::wall;
            } else{
                grid[row][col] = GridElements::empty;
            }
        }
    }
}

bool Grid::updateGrid(int x, int y, GridElements element)
{
    if(y < rows && x < cols)
    {
        grid[y][x] = element;
        return true;
    } 
    return false;
}
