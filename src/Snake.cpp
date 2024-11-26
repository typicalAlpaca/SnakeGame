#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Game/Snake.h"
#include "Game/Grid.h"
#include "config.h"

typedef Grid::GridElements SnakePart;

Snake::Snake(SDL_Renderer *renderer, const char* snakeDir, int width, int height, int row, int col)
    : WIDTH(width), HEIGHT(height), ROW(row), COL(col)
{
    SS_CELL_H = HEIGHT / ROW;
    SS_CELL_W = WIDTH / COL;

    init(renderer, snakeDir);
}

Snake::~Snake()
{
    // Free up memory of all nodes
    while(tail){
        Node* toDelete = tail;
        tail = tail->next;
        delete toDelete;
    }

}

void Snake::initSnakeSpriteSheet()
{
    snakePartsRect[SnakePart::bodyNE] = {0*SS_CELL_W, 1*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakePart::bodyNW] = {2*SS_CELL_W, 2*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakePart::bodyNS] = {2*SS_CELL_W, 1*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakePart::bodySE] = {0*SS_CELL_W, 0*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakePart::bodySW] = {2*SS_CELL_W, 0*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakePart::bodyEW] = {1*SS_CELL_W, 0*SS_CELL_H, SS_CELL_W, SS_CELL_H};

    snakePartsRect[SnakePart::headN] = {3*SS_CELL_W, 0*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakePart::headS] = {4*SS_CELL_W, 1*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakePart::headW] = {3*SS_CELL_W, 1*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakePart::headE] = {4*SS_CELL_W, 0*SS_CELL_H, SS_CELL_W, SS_CELL_H};

    // Tail direction opposite of tail-end
    snakePartsRect[SnakePart::tailN] = {3*SS_CELL_W, 2*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakePart::tailS] = {4*SS_CELL_W, 3*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakePart::tailW] = {3*SS_CELL_W, 3*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakePart::tailE] = {4*SS_CELL_W, 2*SS_CELL_H, SS_CELL_W, SS_CELL_H};
}

void Snake::init(SDL_Renderer *renderer, const char* snakeDir)
{
    // Initialize head and tail nodes
    head = new Node({Grid::rows/2, Grid::cols/2}, SnakePart::headE, Direction::E);
    tail = new Node({Grid::rows/2-2, Grid::cols/2}, SnakePart::tailE, Direction::E);
    tail->next = new Node({Grid::rows/2-1, Grid::cols/2}, SnakePart::bodyEW, Direction::E, head);
    
    initSnakeSpriteSheet();

    SDL_Surface* tempSurface = IMG_Load(snakeDir);
    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 255, 255, 255));
    snakeTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    SDL_Log("Snake initialized");
}

enum Direction{N, S, E, W};

void Snake::update(Direction dir, bool eaten)
{
    // Update head
    switch(dir){
        case Direction::N:
            goNorth(head);
            break;
        case Direction::S:
            goSouth(head);
            break;
        case Direction::W:
            goWest(head);
            break;
        case Direction::E:
            goEast(head);
            break;
        case Direction::SAME:
            update(head->dir, eaten);
            return;
    }

    // Move tail node
    if(!eaten)
    {
        updateTailNode(tail);
    }
}

// Render method updates Grid object with Snake parts
void Snake::render(Grid *grid)
{
    Node* temp = tail;
    while(temp){
        if(!grid->updateGrid(temp->coordinates.first, temp->coordinates.second, temp->part)){
            SDL_Log("Encountered error with rendering snake");
        }
        temp = temp->next;
    }
}

void Snake::updateBodyNode(Node*& body)
{
    Direction currDir = body->dir;
    Direction nextDir = body->next->dir;

    // Look at current direction and next direction to determine current body part.
    if(currDir == nextDir)
    {
        // Both same direction means simply convert to body part in same direction
        switch(currDir){
            case Direction::N:
            case Direction::S:
                body->part = SnakePart::bodyNS;                         // NS
                break;
            case Direction::W:
            case Direction::E:
                body->part = SnakePart::bodyEW;                         // EW
                break;
            default:
                break;
        }
    }
    else
    {
        // Different direction means join both directions
        if((currDir == Direction::N && nextDir == Direction::E)         // SE
            || (currDir == Direction::W && nextDir == Direction::S))
        {
            body->part = SnakePart::bodySE;
        }
        else if ((currDir == Direction::N && nextDir == Direction::W)   // SW
            || (currDir == Direction::E && nextDir == Direction::S))
        {
            body->part = SnakePart::bodySW;
        }
        else if ((currDir == Direction::S && nextDir == Direction::E)   // NE
            || (currDir == Direction::W && nextDir == Direction::N))
        {
            body->part = SnakePart::bodyNE;
        }
        else if ((currDir == Direction::S && nextDir == Direction::W)   // NW
            || (currDir == Direction::E && nextDir == Direction::N))
        {
            body->part = SnakePart::bodyNW;
        }
    }
}

void Snake::goNorth(Node*& head){
    if(head->part == SnakePart::headS)
    {
        head->next = new Node({head->coordinates.first, (head->coordinates.second+1)}, SnakePart::headS, Direction::S);
    } 
    else
    {
        head->next = new Node({head->coordinates.first, (head->coordinates.second-1)}, SnakePart::headN, Direction::N);
    }
    updateBodyNode(head);
    head = head->next;
}

void Snake::goSouth(Node*& head){
    if(head->part == SnakePart::headN)
    {
        head->next = new Node({head->coordinates.first, (head->coordinates.second-1)}, SnakePart::headN, Direction::N);
    } 
    else
    {
        head->next = new Node({head->coordinates.first, (head->coordinates.second+1)}, SnakePart::headS, Direction::S);
    }
    updateBodyNode(head);
    head = head->next;
}

void Snake::goWest(Node*& head){
    if(head->part == SnakePart::headE)
    {
        head->next = new Node({(head->coordinates.first+1), head->coordinates.second}, SnakePart::headE, Direction::E);
    } 
    else
    {
        head->next = new Node({(head->coordinates.first-1), head->coordinates.second}, SnakePart::headW, Direction::W);
    }
    updateBodyNode(head);
    head = head->next;
}

void Snake::goEast(Node*& head){
    if(head->part == SnakePart::headW)
    {
        head->next = new Node({(head->coordinates.first-1), head->coordinates.second}, SnakePart::headW, Direction::W);
    } 
    else
    {
        head->next = new Node({(head->coordinates.first+1), head->coordinates.second}, SnakePart::headE, Direction::E);
    }
    updateBodyNode(head);
    head = head->next;
}

void Snake::updateTailNode(Node*& tail)
{
    Node* toDelete = tail;
    tail = tail->next;
    delete toDelete;
    switch(tail->next->dir){
        case Direction::N:
            tail->part = SnakePart::tailN;
            break;
        case Direction::S:
            tail->part = SnakePart::tailS;
            break;
        case Direction::W:
            tail->part = SnakePart::tailW;
            break;
        case Direction::E:
            tail->part = SnakePart::tailE;
            break;
        default:
            break;
    }
}
