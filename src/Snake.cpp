#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Game/Snake.h"
#include "Game/Grid.h"
#include "config.h"

Snake::Snake(SDL_Renderer *renderer)
{
    SS_CELL_H = c_snakeHeight / c_snakeRow;
    SS_CELL_W = c_snakeWidth / c_snakeCol;

    init(renderer);
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
    snakePartsRect[SnakeParts::bodyNE] = {0*SS_CELL_W, 1*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakeParts::bodyNW] = {2*SS_CELL_W, 2*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakeParts::bodyNS] = {2*SS_CELL_W, 1*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakeParts::bodySE] = {0*SS_CELL_W, 0*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakeParts::bodySW] = {2*SS_CELL_W, 0*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakeParts::bodyEW] = {1*SS_CELL_W, 0*SS_CELL_H, SS_CELL_W, SS_CELL_H};

    snakePartsRect[SnakeParts::headN] = {3*SS_CELL_W, 0*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakeParts::headS] = {4*SS_CELL_W, 1*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakeParts::headW] = {3*SS_CELL_W, 1*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakeParts::headE] = {4*SS_CELL_W, 0*SS_CELL_H, SS_CELL_W, SS_CELL_H};

    // Tail direction opposite of tail-end
    snakePartsRect[SnakeParts::tailN] = {3*SS_CELL_W, 2*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakeParts::tailS] = {4*SS_CELL_W, 3*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakeParts::tailW] = {3*SS_CELL_W, 3*SS_CELL_H, SS_CELL_W, SS_CELL_H};
    snakePartsRect[SnakeParts::tailE] = {4*SS_CELL_W, 2*SS_CELL_H, SS_CELL_W, SS_CELL_H};
}

void Snake::init(SDL_Renderer *renderer)
{
    // Initialize head and tail nodes
    head = new Node({Grid::rows/2, Grid::cols/2}, SnakeParts::headE, Direction::E);
    tail = new Node({Grid::rows/2-2, Grid::cols/2}, SnakeParts::tailE, Direction::E);
    tail->next = new Node({Grid::rows/2-1, Grid::cols/2}, SnakeParts::bodyEW, Direction::E, head);
    
    initSnakeSpriteSheet();

    SDL_Surface* tempSurface = IMG_Load(c_SNAKEDIR);
    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 255, 255, 255));
    snakeTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    SDL_Log("Snake initialized");
}

enum Direction{N, S, E, W};

void Snake::update(Direction dir, bool& grow)
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
            update(head->dir, grow);
            return;
    }

    // Move tail node if food has not been consumed
    if(grow) {
        grow = false;
    } else {
        updateTailNode(tail);
    }
}

// Render method updates Grid object with Snake parts
void Snake::render(SDL_Renderer* renderer)
{
    Node* temp = tail;
    while(temp){
        SDL_Rect destR = {temp->coord.first*c_CELLSIZE + c_BORDERTHICKNESS, 
                          temp->coord.second*c_CELLSIZE + c_BORDERTHICKNESS, 
                          c_CELLSIZE, c_CELLSIZE};
        SDL_RenderCopy(renderer, snakeTexture, &snakePartsRect[temp->part], &destR);
        temp = temp->next;
    }
}

std::pair<int, int> Snake::getHead()
{
    return head->coord;
}

void Snake::populateGrid(Grid *grid)
{
    Node* temp = tail;
    while(temp){
        grid->updateGrid(temp->coord.first, temp->coord.second, Grid::GridElements::snake);
        temp = temp->next;
    }
}


void Snake::goNorth(Node*& head){
    if(head->part == SnakeParts::headS)
    {
        head->next = new Node({head->coord.first, (head->coord.second+1)}, SnakeParts::headS, Direction::S);
    } 
    else
    {
        head->next = new Node({head->coord.first, (head->coord.second-1)}, SnakeParts::headN, Direction::N);
    }
    updateBodyNode(head);
    head = head->next;
}

void Snake::goSouth(Node*& head){
    if(head->part == SnakeParts::headN)
    {
        head->next = new Node({head->coord.first, (head->coord.second-1)}, SnakeParts::headN, Direction::N);
    } 
    else
    {
        head->next = new Node({head->coord.first, (head->coord.second+1)}, SnakeParts::headS, Direction::S);
    }
    updateBodyNode(head);
    head = head->next;
}

void Snake::goWest(Node*& head){
    if(head->part == SnakeParts::headE)
    {
        head->next = new Node({(head->coord.first+1), head->coord.second}, SnakeParts::headE, Direction::E);
    } 
    else
    {
        head->next = new Node({(head->coord.first-1), head->coord.second}, SnakeParts::headW, Direction::W);
    }
    updateBodyNode(head);
    head = head->next;
}

void Snake::goEast(Node*& head){
    if(head->part == SnakeParts::headW)
    {
        head->next = new Node({(head->coord.first-1), head->coord.second}, SnakeParts::headW, Direction::W);
    } 
    else
    {
        head->next = new Node({(head->coord.first+1), head->coord.second}, SnakeParts::headE, Direction::E);
    }
    updateBodyNode(head);
    head = head->next;
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
                body->part = SnakeParts::bodyNS;                         // NS
                break;
            case Direction::W:
            case Direction::E:
                body->part = SnakeParts::bodyEW;                         // EW
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
            body->part = SnakeParts::bodySE;
        }
        else if ((currDir == Direction::N && nextDir == Direction::W)   // SW
            || (currDir == Direction::E && nextDir == Direction::S))
        {
            body->part = SnakeParts::bodySW;
        }
        else if ((currDir == Direction::S && nextDir == Direction::E)   // NE
            || (currDir == Direction::W && nextDir == Direction::N))
        {
            body->part = SnakeParts::bodyNE;
        }
        else if ((currDir == Direction::S && nextDir == Direction::W)   // NW
            || (currDir == Direction::E && nextDir == Direction::N))
        {
            body->part = SnakeParts::bodyNW;
        }
    }
}

void Snake::updateTailNode(Node*& tail)
{
    Node* toDelete = tail;
    tail = tail->next;
    delete toDelete;
    switch(tail->next->dir){
        case Direction::N:
            tail->part = SnakeParts::tailN;
            break;
        case Direction::S:
            tail->part = SnakeParts::tailS;
            break;
        case Direction::W:
            tail->part = SnakeParts::tailW;
            break;
        case Direction::E:
            tail->part = SnakeParts::tailE;
            break;
        default:
            break;
    }
}
