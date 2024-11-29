#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>

#include "Game/Game.h"
#include "Game/Grid.h"
#include "Game/Food.h"
#include "Game/Walls.h"
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

    // Load and initialize objects
    walls = new Walls();
    grid = new Grid();
    snake = new Snake(renderer);
    food = new Food(renderer);

    walls->populateGrid(grid);
    food->getNewFood(grid, snake, walls);
    initScore();
    initWindows();
}

void Game::initScore()
{
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, scoreText, c_textColor);
    int w = textSurface->w;
    int h = textSurface->h;
    int x = c_WIDTH - w - c_BORDERTHICKNESS_RIGHT;
    int y = c_HEIGHT - h - c_BORDERTHICKNESS_BTM*0.1;

    scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    scoreRect = {x, y, w, h};
}

void Game::initWindows()
{
    SDL_Surface *gameOverSurface = IMG_Load(c_GAMEOVERWINDOWDIR);
    gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_FreeSurface(gameOverSurface);
    gameOverDestR = {
        (c_WIDTH - (int)(c_GAMEOVERWINDOW_RATIO*c_WIDTH))/2,
        (c_HEIGHT - (int)(c_GAMEOVERWINDOW_RATIO*c_HEIGHT))/2,
        (int)(c_GAMEOVERWINDOW_RATIO*c_WIDTH),
        (int)(c_GAMEOVERWINDOW_RATIO*c_HEIGHT)
    };

    SDL_Surface *pausedSurface = IMG_Load(c_PAUSEDDIR);
    pausedTexture = SDL_CreateTextureFromSurface(renderer, pausedSurface);
    SDL_FreeSurface(pausedSurface);
    pausedDestR = {
        (c_WIDTH - (int)(c_PAUSEDWINDOW_RATIO*c_WIDTH))/2,
        (c_HEIGHT - (int)(c_PAUSEDWINDOW_RATIO*c_HEIGHT))/2,
        (int)(c_PAUSEDWINDOW_RATIO*c_WIDTH),
        (int)(c_PAUSEDWINDOW_RATIO*c_HEIGHT)
    };
    
    SDL_Log("Windows initialized");
}

void Game::handleEvents()
{
    SDL_PollEvent(&event);
    switch(event.type){
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            // Key is pressed, only check for arrow keys and WASD
            switch(event.key.keysym.sym){
                // Up (Arrow up || W key)
                case SDLK_UP:
                case SDLK_w:
                    nextDir = Snake::Direction::N;
                    break;
                // Down (Arrow down || S key)
                case SDLK_DOWN:
                case SDLK_s:
                    nextDir = Snake::Direction::S;
                    break;
                // Left (Arrow left || A key)
                case SDLK_LEFT:
                case SDLK_a:
                    nextDir = Snake::Direction::W;
                    break;
                // Right (Arrow right || D key)
                case SDLK_RIGHT:
                case SDLK_d:
                    nextDir = Snake::Direction::E;
                    break;
                // Toggle pause/continue game
                case SDLK_p:
                    if(state == State::pause || state == State::playing){
                        state == State::pause ? SDL_Log("Continue Game") : SDL_Log("Pause Game");
                        state = state == State::pause ? State::playing : State::pause;
                    }
                    break;
                // On game over, Q to quit, R to restart
                case SDLK_r:
                    if(state == State::gameover){
                        state = State::restart;
                        SDL_Log("Pressed R to restart");
                    }
                    break;
                case SDLK_q:
                    if(state == State::pause || state == State::gameover){
                        isRunning = false;
                        SDL_Log("Pressed Q to quit");
                    }
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
    switch(state){
        case State::playing:
            snake->update(nextDir, grow);
            checkFood();
            grid->update(snake, food);
            resetVariables();
            break;
        case State::pause:
        case State::gameover:
            break;
        case State::restart:
            snake->init(renderer);
            food->getNewFood(grid, snake, walls);
            grid->init();
            walls->populateGrid(grid);
            resetVariables();
            state = State::playing;
            SDL_Log("Restarting...");
            break;
        default:
            break;
    }
}

void Game::render()
{
    switch(state){
        case State::playing:
            SDL_SetRenderDrawColor(renderer, c_bgColor.r, c_bgColor.b, c_bgColor.g, c_bgColor.a);
            SDL_RenderClear(renderer);

            // Pass in stuff to render
            walls->render(renderer);
            snake->render(renderer);
            food->render(renderer);
            grid->render(renderer);
            renderScore();

            SDL_RenderPresent(renderer);
            break;
        case State::pause:
            SDL_RenderCopy(renderer, pausedTexture, NULL, &pausedDestR);
            SDL_RenderPresent(renderer);
            break;
        case State::gameover:
            SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverDestR);
            SDL_RenderPresent(renderer);
            break;
        case State::restart:
            break;
        default:
            break;
    }
}

void Game::doChecks()
{
    if(grid->collide && state != State::gameover){
        state = State::gameover;
        SDL_Log("Collision detected, game over");
    }
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
    nextDir = Snake::Direction::SAME;
    if(state == State::restart)
    {
        score = 0;
        updatedScore = true;
    }
}

void Game::checkFood()
{
    if(snake->getHead() == food->coord){
        score++;
        updatedScore = true;
        food->getNewFood(grid, snake, walls);
        grow = true;
    }
}


void Game::renderScore()
{
    if(updatedScore)
    {
        updatedScore = false;
        snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
        scoreTexture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, scoreText, c_textColor));
    }
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
}