#ifndef __PIECE_H__
#define __PIECE_H__

#include "Entity.h"
#include "Block.h"
#include "CircularBuffer.h"

#include <vector>

/*
    PieceShape is a CircularBuffer contain shape and rotation of it. Each shape
    made by vector of Point(x,y)
    eg:
    The T shape:
    @
    @@@ => the body is [(0, 0), (1, 0), (1, 1), (2, 0)].
*/
using PieceShape = CircularBuffer<std::vector<Point>>;

class Piece {
  public:

    /* Defines a new Piece by given the Block (Texture object to render block of
       shape) and pointer to PieceShape.
    */
    Piece(PieceShape* body);

    /* Make new piece by random index of PicesLib */
    Piece();

        /* Make new piece by random index of PicesLib */
    Piece(int index);

    ~Piece();

    /* Get width of Piece: the max x of body*/
    int getWidth();

    /* Get Height of Piece: The max y of body*/
    int getHeight();

    /* Access to the body*/
    std::vector<Point>& getBody();

    /* Returns a pointer to the piece's skirt. For each x value
      across the piece, the skirt gives the lowest y value in the body.
      This useful for computing where the piece will land.
      The caller should not modify this array.
    */
    std::vector<int>& getSkirt();

    /* Rotation pieceShapde by 90 deg*/
    Piece* nextRotation();

    /* Return number rotations of each shape */
    int numOfRotation();

    /* Helper function */
    bool operator==(Piece& other);

    /* Vector contain all pre-computes shape and rotation to get and rotation in
        constant time
    */

    static Piece* getPiece();

  private:
    int width_;
    int heigth_;

    PieceShape* body_;

    std::vector<int> skirt_;

    static std::vector<PieceShape*> PicesLib;
};

#endif
