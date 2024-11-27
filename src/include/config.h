#ifndef CONFIG_H
#define CONFIG_H

#include <SDL2/SDL_pixels.h>

constexpr const char* c_title = "Snake Gameee";
constexpr const int c_WINDOWWIDTH = 1000;
constexpr const int c_WINDOWHEIGHT = 900;
constexpr const int c_CELLSIZE = 50;
constexpr const int c_BORDERTHICKNESS = 10;

constexpr const int c_WIDTH = c_WINDOWWIDTH + 2*c_BORDERTHICKNESS;
constexpr const int c_HEIGHT = c_WINDOWHEIGHT + 2*c_BORDERTHICKNESS;
constexpr SDL_Color c_bgColor = {50, 50, 50, 0};                // Dark Gray
constexpr SDL_Color c_wallColor = {100, 100, 100, 255};         // Light Gray

constexpr const int c_FPS = 120;
constexpr const int c_ticksPerSec = 6;

constexpr const char* c_FONTDIR = "fonts/Roboto-Regular.ttf";
constexpr const int c_fontSize = 20;

constexpr const char* c_SNAKEDIR = "assets/snake.png";
constexpr const int c_snakeWidth = 320;
constexpr const int c_snakeHeight = 256;
constexpr const int c_snakeRow = 4;
constexpr const int c_snakeCol = 5;

#endif
