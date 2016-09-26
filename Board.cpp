#include "Board.h"
#include <iostream>
#include <vector>
#include <algorithm>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

Board::Board(SDL_Renderer* renderer, int row, int column)
    : Entity(renderer), row_(row), column_(column) {

    /*Create grid is 2D-array row x column */
    grid_ = GridBoard(row_, std::vector<bool>(column_, false));

    vHeight_ = std::vector<int> (column_, 0);
    bHeight_ = vHeight_;

    vWidth_ = std::vector<int> (row_, 0);
    bWidth_ = vWidth_;

    maxHeight_ = 0;
    bMaxHeitht_ = 0;

    /*First state of commit is false*/
    committed_ = true;

    /*Load texture*/
    SDL_Surface* img = IMG_Load("img/board.png");
    board_texture_ = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);

    /* Size of texture tobe render */
    x_ = 0;
    y_ = 0;
    width_ = BOARD_WIDTH;
    height_ = BOARD_HEIGHT;
}


Board::~Board() {
    SDL_DestroyTexture(board_texture_);
}

// void Board::update(Uint32 dt) {
//      Board fixed position, don't need update
// }

void Board::render() {
    SDL_Rect dstrect;
    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = width_;
    dstrect.h = height_;
    SDL_RenderCopy(renderer_, board_texture_, 0, &dstrect);

    for (int i = 0; i < row_ - BOARD_INVISIBLE_SIZE; ++i)
        for (int j = 0; j < column_; ++j ) {
            if (grid_[i][j])
                Block::getInstance(renderer_)->render(Point(i, j));
        }
}


bool Board::getGrid(int row, int column) {
    return grid_[row][column];
}

/*
 Returns the height of the given column --
 i.e. the y value of the highest block + 1.
 The height is 0 if the column contains no blocks.
*/
int Board::getColumnHeight(int column) {
    return vHeight_[column];
}

/*
 Returns the max column height present in the board.
 For an empty board this is 0.
*/
int Board::getMaxHeight() {
    return maxHeight_;
}


int Board::getRowWidth(int row) {
    return vWidth_[row];
}

bool Board::isFilledRow(int row) {
    for (auto elem : grid_[row])
        if (!elem) return false;
    return true;
}

/* Place a piece at point p of board. The origin of piece (the lower-left corner)
   located at p.

    return PLACE_OK for successful placement
          PLACE_ROW_FILLED for succsseful placement that at least one row to
          become filled.
    return PLACE_OUT_BOUNDS if piece out of bound
    return PLACE_BAD if block of body overlaps already filled spots.
*/
int Board::place(Piece* piece, Point p) {
    /* Only place when state is commited */
    if (!committed_)
        return PLACE_BAD;

    committed_ = false;

    /* Backup grid */
    grid_backup_ = grid_;
    bWidth_ = vWidth_;
    bHeight_ = vHeight_;
    bMaxHeitht_ = maxHeight_;
    if ( p.col < 0 || piece->getWidth()  + p.col >= column_
            || piece->getHeight() + p.row >= row_    ) {
        return PLACE_OUT_BOUNDS;
    }

    bool rowFilled = false; // Flag for detect filled row

    if (p.row < 0)
        return PLACE_BAD;

    // Places every point on the grid
    for (int k = 0; k < piece->getBody().size(); k++) {

        int currentRow = piece->getBody()[k].row + p.row;
        int currentCol = piece->getBody()[k].col + p.col;

        if (grid_[currentRow][currentCol]) {
            return PLACE_BAD;
        } else {

            grid_[currentRow][currentCol] = true;

            //increments the widths arrays
            vWidth_[currentRow]++;

            //this increases heights to the correct heights
            //note the +1 to compensate for index starting at zero

            if (vHeight_[currentCol] < currentRow + 1 ) {
                vHeight_[currentCol] = currentRow + 1;

                //increases maxHeight if need be
                if (maxHeight_ < vHeight_[currentCol])
                    maxHeight_ = vHeight_[currentCol];
            }

            //flags a variable if the widths is full
            if ( vWidth_[currentRow] == column_)
                rowFilled = true;
        }
    }

    if (rowFilled == true) {
        return PLACE_ROW_FILLED;
    }

    return PLACE_OK;

}

void Board::clear() {

    GridBoard emptyBoard = GridBoard(row_, std::vector<bool>(column_, false));

    grid_.swap(emptyBoard);
    grid_backup_.swap(emptyBoard);

    std::fill(vHeight_.begin(), vHeight_.end(), 0);
    std::fill(vWidth_.begin() , vWidth_.end(), 0);

    maxHeight_ = 0;
    bMaxHeitht_ = 0;

    committed_ = true;
}

/* Delete each row that filled all the way across, causing things above to shift down*/
/* Return numbers of row filed! */
int Board::clearRow() {
    if (!committed_) {
        committed_ = false;

        /* Backup data*/
        grid_backup_ = grid_;
        bWidth_ = vWidth_;
        bHeight_ = vHeight_;
    }

    unsigned int rowsDeleted = 0;
    for (int i = 0; i < vWidth_.size(); ++i) {
        /* Filled row*/
        if (vWidth_[i] == column_) {
            vWidth_.erase(i + vWidth_.begin());
            vWidth_.push_back(0);

            grid_.erase(i + grid_.begin());
            grid_.push_back(std::vector<bool> (column_, false));

            for (int i = 0; i < vHeight_.size(); ++i) {
                if (vHeight_[i] > 0) --vHeight_[i];
            }

            --maxHeight_;

            rowsDeleted++;
        }
    }

    return rowsDeleted;

}

/* Compute the y values where origin (0,0) of piece will come to rest if dropped
    in the given column from infinitely high.
*/

int Board::dropHeight(Piece* piece, int column) {
    int toReturn = 0;
    //this goes through the entire skirt and calculates what drop height
    //is dictated by each piece, and sets toreturn to the highest
    for(int k = 0; k < piece->getSkirt().size(); ++k){
        //if the piece given doe not fit on the board
        if(k + column >= column_){
            std::cout << "ilegal piece given to dropHeight" << std::endl;
        } else if(vHeight_[k+column] - piece->getSkirt()[k] > toReturn){
            toReturn = vHeight_[k + column] - piece->getSkirt()[k];
        }
    }
    return toReturn;
}

/*  1-deep undo facility. It can be used for animation of falling shape*/
void Board::undo() {
    if (!committed_) {
        grid_.swap(grid_backup_);
        vHeight_.swap(bHeight_);
        vWidth_.swap(bWidth_);
        maxHeight_ = bMaxHeitht_;

        committed_ = true;
    }
}

/*  Puts the board in the committed state.*/
void Board::commit() {
    committed_ = true;
}

/* Gen random grid for testing */
void Board::genRandomGrid() {
    srand(time(0));
    int row = rand() % row_ - 1;
    for (int i = 1; i < row; i++) {
        std::vector<bool> v;
        srand(time(0));
        bool ret;
        for (int i = 0; i < column_; ++i) {
            ret = (rand() % 2 == 0) ? 0 : 1;
            v.push_back(ret);
        }
        grid_[i].swap(v);
    }

    std::vector<bool> filled(column_, true);
    grid_[0].swap(filled);
}
