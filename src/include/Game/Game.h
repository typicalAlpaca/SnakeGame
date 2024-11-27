#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Game/Snake.h"

//Forward declarations
class Grid;
class Food;
class Walls;


class Game{

public:
    Game();
    ~Game();

    enum State{playing, pause, gameover, restart};

    void init(const char* title, int xpos, int ypos, int width, int height, int flags = 0);

    void handleEvents();    // Handles all events
    void update();          // Updates all objects
    void render();          // Renders the display with the new frame
    void clean();           // Clear memory of removed objects
    bool running() {return isRunning;}

public:
    bool grow = false;
    void checkFood();

private:
    bool isRunning;
    int score = 0;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    TTF_Font *font;

    Snake *snake;
    Walls *walls;
    Grid *grid;
    Food *food;
    State state = State::playing;
    Snake::Direction nextDir = Snake::Direction::E;

    void resetVariables();
};

#endif // GAME_H
