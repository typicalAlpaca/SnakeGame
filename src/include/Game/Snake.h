#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <utility>

// Forward declarations
class Grid;


class Snake{

public:
    enum Direction{N, S, E, W, SAME};
    enum SnakeParts{
        bodyNE, bodyNW, bodyNS, bodySE, bodySW, bodyEW,             // Snake body parts
        headN, headS, headW, headE,                                 // Snake head parts
        tailN, tailS, tailW, tailE                                  // Snake tail parts
    };

    // Snake is stored as a linked list that can grow.
    struct Node{
        std::pair<int, int> coord;    // x, y
        SnakeParts part;
        Direction dir;
        Node* next;

        Node(std::pair<int, int> crd, SnakeParts prt, Direction direction) : coord(crd), part(prt), dir(direction), next(nullptr) {}
        Node(std::pair<int, int> crd, SnakeParts prt, Direction direction, Node* Next) : coord(crd), part(prt), dir(direction), next(Next) {}
    };

private:
    Node* head;
    Node* tail;
    SDL_Rect snakePartsRect[14];
    SDL_Texture* snakeTexture;

public:
    Snake(SDL_Renderer* renderer);
    ~Snake();

    int length = 3;

    void init(SDL_Renderer* renderer);
    void update(Direction dir, bool& grow);
    void render(SDL_Renderer* renderer);
    std::pair<int, int> getHead();
    void populateGrid(Grid *grid);

private:
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
