#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Game/Game.h"
#include "config.h"

Game::Game()
{
    init(c_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, c_WIDTH, c_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Log("Snake Game initialized");
}

Game::~Game()
{}

void Game::init(const char *title, int xpos, int ypos, int width, int height, int flags)
{
    isRunning = false;

    // Initialize SDL subsystems
    if(SDL_InitSubSystem(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Failed to initialize SDL subsystems : %s", SDL_GetError());
        return;
    }

    // Initialize SDL_TTF font reader
    if(TTF_Init() == -1){
        SDL_Log("Text fonts failed to load : %s", SDL_GetError());
        return;
    }
    font = TTF_OpenFont(c_FONTDIR, c_fontSize);

    // Create SDL Window surface
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if(!window)
    {
        SDL_Log("Failed to create window : %s", SDL_GetError());
        return;
    }

    // Create SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {
        SDL_Log("Failed to initialize renderer : %s", SDL_GetError());
        return;
    }

    isRunning = true;

    // Load grid
    grid = new Grid();

    // Load snake
    snake = new Snake(renderer, c_SNAKEDIR, c_snakeWidth, c_snakeHeight, c_snakeRow, c_snakeCol);

    // Extra test shit
    SDL_SetRenderDrawColor(renderer, c_bgColor.r, c_bgColor.b, c_bgColor.g, c_bgColor.a);   // bg color
}

void Game::handleEvents()
{
    SDL_PollEvent(&event);
    switch(event.type){
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            // Key is pressed, only check for arrow keys
            switch(event.key.keysym.sym){
                case SDLK_UP:
                    nextDir = Snake::Direction::N;
                    break;
                case SDLK_DOWN:
                    nextDir = Snake::Direction::S;
                    break;
                case SDLK_LEFT:
                    nextDir = Snake::Direction::W;
                    break;
                case SDLK_RIGHT:
                    nextDir = Snake::Direction::E;
                    break;
                default:
                    break;
            }
        
        default:
            break;
    }
}

void Game::update()
{
    snake->update(nextDir, eaten);
    grid->update(snake);
    resetVariables();
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, c_bgColor.r, c_bgColor.b, c_bgColor.g, c_bgColor.a);
    SDL_RenderClear(renderer);

    // Pass in stuff to render
    snake->render(grid);
    grid->render(renderer, snake);

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_Quit();
    SDL_Log("Game cleaned");
}

void Game::resetVariables()
{
    eaten = false;
    nextDir = Snake::Direction::SAME;
}
