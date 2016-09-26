#ifndef __BOARD_H__
#define __BOARD_H__

#include "Entity.h"
#include "Piece.h"
#include "Texture.h"
#include <vector>

const int BOARD_OFFSET = 6;
const int BOARD_HEIGHT = 480 + BOARD_OFFSET * 2;
const int BOARD_WIDTH  = 340 + BOARD_OFFSET * 2;

const int BOARD_ROW = 28;
const int BOARD_COL = 17;

const int BOARD_INVISIBLE_SIZE = 4;

const int BOARD_ROW_ORG = BOARD_ROW - BOARD_INVISIBLE_SIZE;
const int BOARD_COL_ORG = BOARD_COL;

const int NEXT_PIECE_POS_ROW = 20;
const int NEXT_PIECE_POS_COL = 18;

const int SCORE_POS_ROW = 370;
const int SCORE_POS_COL = 150;

/* Using 2D vector bool to descript grid board*/
using GridBoard = std::vector<std::vector<bool>>;

class Board : public Entity {
  public:
    /* Create board witch zize row x column (block) */
    Board(SDL_Renderer* renderer, int row, int column);

    virtual ~Board();

    void update() {};

    void render();

    int getColumn() { return column_;}

    int getRow() { return row_;}

    /* Place a piece at point p of board. The origin of piece (the lower-left corner)
       located at p.

        return PLACE_OK for successful placement
              PLACE_ROW_FILLED for succsseful placement that at least one row to
              become filled.
        return PLACE_OUT_BOUNDS if piece out of bound
        return PLACE_BAD if block of body overlaps already filled spots.
    */
    int place(Piece* piece, Point p);

    /* Delete each row that filled all the way across, causing things above to shift down*/
    int clearRow();

    /* Compute the y values where origin (0,0) of piece will come to rest if dropped
        in the given column from infinitely high.
    */
    int dropHeight(Piece* piece, int column);

    /*  1-deep undo facility. It can be used for animation of falling shape*/
    void undo();

    /*  Puts the board in the committed state.*/
    void commit();

    /*
     Returns the height of the given column --
     i.e. the y value of the highest block + 1.
     The height is 0 if the column contains no blocks.
    */

    int getColumnHeight(int column);

      /*
     Returns the max column height present in the board.
     For an empty board this is 0.
    */
    int getMaxHeight();

    /*
     Returns the number of filled blocks in
     the given row.
    */
    int getRowWidth(int y);

    bool getGrid(int row, int column);

    void clear();

    /* Testing */
    void genRandomGrid();

    bool isCommited() { return committed_;}

    enum PlaceStatus {
        PLACE_OK,
        PLACE_ROW_FILLED,
        PLACE_OUT_BOUNDS,
        PLACE_BAD,
    };

  private:
    SDL_Texture* board_texture_;
    int row_;
    int column_;
    GridBoard grid_;
    GridBoard grid_backup_;

    std::vector<int> vHeight_; //the height of each coloumn
    std::vector<int> vWidth_;  //the number of blocks in each row

    std::vector<int> bHeight_; // Backup of vHeight
    std::vector<int> bWidth_;  // Backup of vWidth

    int maxHeight_;
    int bMaxHeitht_;
    bool committed_;

    int        score_;
    Texture*   scoreTexture_;
    /* Check filled row */
    bool isFilledRow(int row);
};

#endif
