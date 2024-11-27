#ifndef FOOD_H
#define FOOD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <utility>
#include <random>

// Class Forward Declarations
class Grid;
class Snake;
class Walls;


class Food{

public:
    Food(SDL_Renderer *renderer);
    ~Food();

    void init(SDL_Renderer *renderer);
    void update(Grid *grid, Snake *snake);          // Updates all objects
    void render(SDL_Renderer *renderer);          // Renders the display with the new frame
    void getNewFood(Grid *grid, Snake *snake, Walls *walls);
    void populateGrid(Grid *grid);

public:
    bool eaten = false;
    std::pair<int, int> coord;    // x, y

private:
    std::default_random_engine engine;

    SDL_Rect foodRect = {0*64, 3*64, 64, 64}; // Hardcoded for now, to update with image.
    SDL_Texture* foodTexture;

    void initRandomEngine();

};

#endif // FOOD_H
