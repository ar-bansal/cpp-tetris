#include "board.h"


#ifndef PIECES_H
#define PIECES_H

#include <vector>


class Board;


class Piece{
public:

    struct BoundingBox {
        int xmin = -1;
        int xmax = -1;
        int ymin = -1;
        int ymax = -1;
    };
    BoundingBox location;

    struct Shape {
        std::vector<std::vector<char>> grid;
        int height; // = grid.size();
        int width; // = grid.at(0).size();
    };
    Shape shape;
    char blankSpace;

    bool isFrozen;
    
    Piece(char blank);

    void transposeDiagonal();
    void transposeAntidiagonal();
    void flip();

    void rotateRight();
    void rotateLeft();

    bool isValidMoveRight(const Board& board);
    void moveRight(const Board& board);

    bool isValidMoveLeft(const Board& board);
    void moveLeft(const Board& board);

    bool isValidMoveDown(const Board& board);
    void moveDown(Board& board);

    void freeze(Board& board);

    // virtual ~Piece();
};


class IPiece : public Piece {
public: 
    IPiece(char blank, char fill);
};

class JPiece : public Piece {
public: 
    JPiece(char blank, char fill);
};

class LPiece : public Piece {
public: 
    LPiece(char blank, char fill);
};

class OPiece : public Piece {
public: 
    OPiece(char blank, char fill); 
};

class SPiece : public Piece {
public: 
    SPiece(char blank, char fill);
};

class TPiece : public Piece {
public: 
    TPiece(char blank, char fill);
};

class ZPiece : public Piece {
public: 
    ZPiece(char blank, char fill);
};

#endif