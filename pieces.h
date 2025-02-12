#include "board.h"

#ifndef PIECES_H
#define PIECES_H

#include <vector>
#include <cstdint>
#include <unordered_map>


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
        int height;
        int width;
    };
    Shape shape;
    char blankSpace;

    bool isFrozen;
    
    Piece(char blank);

    void transposeDiagonal();
    void transposeAntidiagonal();
    void flip();

    void rotateRight(Board& board);
    void rotateLeft(Board& board);

    bool isValidMoveRight(Board& board);
    void moveRight(Board& board);
    bool isValidMoveLeft(Board& board);
    void moveLeft(Board& board);
    bool isValidMoveDown(Board& board);
    void moveDown(Board& board);

    bool isRotationValid(Board& board);


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


class PieceGenerator {
private: 
    struct Xorshift {
        uint32_t state;
        int generate();

        Xorshift(uint32_t state) : state(state) {}
    };

    Xorshift rng;
    std::unordered_map<int, Piece> pieceMap;
public: 
    PieceGenerator(char blank, char fill, uint32_t state);

    Piece getPiece();
};


#endif