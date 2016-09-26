#include "Piece.h"
#include <algorithm>

std::vector<PieceShape*> Piece::PicesLib = {
    /*The stick*/
    new PieceShape{
        {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}}
    },
    /*The l Right*/
    new PieceShape{
        {{0, 0}, {0, 1}, {0, 2}, {1, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {2, 1}},
        {{0, 2}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 0}, {0, 1}, {1, 1}, {2, 1}}
    },
    /*The L Left */
    new PieceShape{
        {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 1}, {1, 1}, {2, 0}, {2, 1}},
        {{0, 0}, {0, 1}, {0, 2}, {1, 2}},
        {{0, 0}, {0, 1}, {1, 0}, {2, 0}}
    },
    /*The dog right*/
    new PieceShape{
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
        {{0, 1}, {0, 2}, {1, 0}, {1, 1}}
    },
    /*The dog left*/
    new PieceShape{
        {{0, 1}, {1, 0}, {1, 1} , {2, 0}},
        {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
    },
    /* The T*/
    new PieceShape{
        {{0, 0}, {1, 0}, {1, 1}, {2, 0}},
        {{0, 1}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 1}, {1, 0}, {1, 1}, {2, 1}},
        {{0, 0}, {0, 1}, {0, 2}, {1, 1}}
    },
    /* The Square*/
    new PieceShape{
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
    }
};


Piece::Piece(PieceShape* body) : body_{body}
{}

Piece::Piece() {
    srand(time(0));
    body_ = PicesLib[rand() % PicesLib.size()];

    int index = rand() % body_->size();
    while(index--)
        body_->next();
}

Piece::Piece(int index) {
    body_ = PicesLib[index % PicesLib.size()];

    int rot = rand() % body_->size();
    while(rot--)
        body_->next();
}

Piece::~Piece() {

}

Piece* Piece::getPiece(){
    srand(time(0));
    return  new Piece(PicesLib[rand() % PicesLib.size()]);
}


int Piece::getWidth() {
    auto ret =  std::max_element(body_->get().begin(), body_->get().end(), \
                                [](Point a, Point b) { return (a.col < b.col);}
                );
    return (*ret).col;
}


int Piece::getHeight() {
    auto ret =  std::max_element(body_->get().begin(), body_->get().end(),\
                                [](Point a, Point b) { return (a.row < b.row);}
     );
    return (*ret).row;
}


std::vector<Point>& Piece::getBody() {
    return body_->get();
}


std::vector<int>& Piece::getSkirt() {
    skirt_.clear();
    int minHeightAtColum;
    int numBlockOfShape = 4;
    for (int i = 0; i < numBlockOfShape; ++i) {
        minHeightAtColum = body_->get()[i].col;
        while (body_->get()[i].row == body_->get()[i + 1].row && i < numBlockOfShape -1 ) {
            minHeightAtColum = (minHeightAtColum < body_->get()[i].col) ? minHeightAtColum : body_->get()[i].row;
            i++;
        }
        skirt_.push_back(minHeightAtColum);
    }

    return skirt_;
}

bool Piece::operator==(Piece& other) {
    for (int i = 0; i < 4; ++i) {
        if (body_->get()[i] != other.getBody()[i])
            return false;
    }
    return true;
}

Piece* Piece::nextRotation() {
    body_->next();
    return this;
}

int Piece::numOfRotation() {
    return body_->size();
}

