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
    void initScore();
    void initWindows();

    void handleEvents();    // Handles all events
    void update();          // Updates all objects
    void render();          // Renders the display with the new frame
    void doChecks();        // Execute checks, like collisions.
    int getScore() {return score;}

    void clean();           // Clear memory of removed objects
    bool running() {return isRunning;}
    State getState() {return state;}

public:
    // Variables for other objects to interact with (e.g. Snake/Food)
    bool grow = false;
    void checkFood();

private:
    bool isRunning;

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

    int score = 0;
    char scoreText[50];
    SDL_Texture *scoreTexture;
    SDL_Rect scoreRect;
    bool updatedScore = false;

    SDL_Texture *gameOverTexture;
    SDL_Rect gameOverDestR;

    SDL_Texture *pausedTexture;
    SDL_Rect pausedDestR;

    void resetVariables();
    void renderScore();
};

#endif // GAME_H
