#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "Entity.h"
#include <vector>

struct Point {
    int row;
    int col;
    Point(int pRow = 0, int pCol = 0) : row(pRow), col(pCol)
    {}

    Point& operator+ (const Point& a){
       row += a.row;
       col += a.col;
       return *this;
    }

    bool operator!= (const Point& a) {
        return !(row == a.row && col == a.col);
    }
};

int const BLOCK_WIDTH  = 20;
int const BLOCK_HEIGHT = 20;

/* Singleton Block to render a block to screen */

class Block : public Entity
{
public:
    ~Block();

    static Block* getInstance(SDL_Renderer* renderer);

    void render(Point p);

private:
    Block(SDL_Renderer* renderer);
    static Block* instance_;
    SDL_Texture* block_texture_;
};

#endif
