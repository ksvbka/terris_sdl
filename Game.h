#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Board.h"
#include "Block.h"
#include "Piece.h"

#include <vector>

//Screen dimension constants
const int SCREEN_WIDTH = BOARD_WIDTH + 100;
const int SCREEN_HEIGHT = BOARD_HEIGHT;

const Uint32 FRAME_TICKS = 100; //ms

const Uint32 DOWN_TIME = 500;
const Uint32 FALLING_TIME = 5;

class Game {
  public:
    Game();
    virtual ~Game();

    void init();

    virtual void run();

  private:
    SDL_Window*     window_;
    SDL_Renderer*   renderer_;
    TTF_Font*       font_;
    SDL_Event       event_;
    bool            quit_;

    Board* board_;
    Piece* current_piece_;
    Piece* next_piece_;

    int        score_;
    Texture*   scoreTexture_;

    /* Current position of piece*/
    Point curr_piece_pos_;

    /* Time of piece failing */
    Uint32 down_time_ = DOWN_TIME;

    /* */
    bool moved_;
    virtual void handleEvent();
    virtual void render();
    virtual void clean();
    virtual void newGame();

    void getNewPiece();

    void display(Piece* piece, Point Position);
    void AI(Uint32 dt);

};

#endif
