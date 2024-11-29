#ifndef CONFIG_H
#define CONFIG_H

#include <SDL2/SDL_pixels.h>

constexpr const char* c_title = "Snake Gameee";
constexpr const int c_WINDOWWIDTH = 800;
constexpr const int c_WINDOWHEIGHT = 600;
constexpr const int c_CELLSIZE = 25;
constexpr const int c_BORDERTHICKNESS_TOP = 10;
constexpr const int c_BORDERTHICKNESS_BTM = 30;
constexpr const int c_BORDERTHICKNESS_LEFT = 10;
constexpr const int c_BORDERTHICKNESS_RIGHT = 10;

constexpr const double c_GAMEOVERWINDOW_RATIO = 0.55;
constexpr const double c_PAUSEDWINDOW_RATIO = 0.45;
constexpr const int c_POPUPWINDOW_BORDERTHICKNESS = 10;

constexpr SDL_Color c_bgColor           = {50, 50, 50, 255};            // Dark Gray
constexpr SDL_Color c_wallColor         = {100, 100, 100, 255};         // Light Gray
constexpr SDL_Color c_winBorderColor    = {245, 222, 179, 255};         // Wheat brown
constexpr SDL_Color c_winColor          = {210, 180, 140, 255};         // Tan brown
constexpr SDL_Color c_textColor         = {255, 255, 255, 255};         // Black

constexpr const int c_FPS = 120;
constexpr const int c_TPS = 10;     // Ticks per second, must be less than FPS.
constexpr const int c_MAXTPS = 30;

constexpr const char* c_FONTDIR = "fonts/Roboto-Regular.ttf";
constexpr const int c_fontSize = 25;

constexpr const char* c_SNAKEDIR = "assets/snake.png";
constexpr const int c_snakeWidth = 320;
constexpr const int c_snakeHeight = 256;
constexpr const int c_snakeRow = 4;
constexpr const int c_snakeCol = 5;

constexpr const char* c_GAMEOVERWINDOWDIR = "assets/GameOverWindow.png";
constexpr const char* c_PAUSEDDIR = "assets/PauseWindow.png";

constexpr const int c_WIDTH = c_WINDOWWIDTH + c_BORDERTHICKNESS_LEFT + c_BORDERTHICKNESS_RIGHT;
constexpr const int c_HEIGHT = c_WINDOWHEIGHT + c_BORDERTHICKNESS_TOP + c_BORDERTHICKNESS_BTM;

#endif

