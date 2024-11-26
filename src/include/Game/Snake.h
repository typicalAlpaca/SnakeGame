#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <utility>

#include "Game/Grid.h"

typedef Grid::GridElements SnakePart;

class Snake{

public:
    enum Direction{N, S, E, W, SAME};
    SDL_Rect snakePartsRect[14];
    SDL_Texture* snakeTexture;

private:
    // Snake is stored as a linked list that can grow.
    struct Node{
        std::pair<int, int> coordinates;    // x, y
        SnakePart part;
        Direction dir;
        Node* next;

        Node(std::pair<int, int> crd, SnakePart prt, Direction direction) : coordinates(crd), part(prt), dir(direction), next(nullptr) {}
        Node(std::pair<int, int> crd, SnakePart prt, Direction direction, Node* Next) : coordinates(crd), part(prt), dir(direction), next(Next) {}
    };

    Node* head;
    Node* tail;

public:
    Snake(SDL_Renderer* renderer, const char* snakeDir, int width, int height, int row, int col);
    ~Snake();

    void init(SDL_Renderer* renderer, const char* snakeDir);
    void update(Direction dir, bool eaten);
    void render(Grid* grid);

private:
    int WIDTH;
    int HEIGHT;
    int ROW;
    int COL;
    int SS_CELL_W;
    int SS_CELL_H;

    void initSnakeSpriteSheet();
    void goNorth(Node*& head);
    void goSouth(Node*& head);
    void goWest(Node*& head);
    void goEast(Node*& head);
    void updateBodyNode(Node*& body);
    void updateTailNode(Node*& tail);
};

#endif // SNAKE_H
