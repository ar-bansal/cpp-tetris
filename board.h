
#ifndef BOARD_H
#define BOARD_H

#include "pieces.h"
#include <vector>
#include <optional>

class Piece;

class Board {
public: 
    std::vector<std::vector<char>> grid;
    
    int width;
    int height;
    char blankSpace;
    
    Piece* currPiece;

    Board(int r, int c, char blank);

    // void display(const Piece& piece);
    void display();
    bool hasSpace(const Piece& piece); 
    // bool hasSpace(); 
    void insertPiece(Piece& piece);

    bool isLineFull(int l);
    bool isLineEmpty(int l);
    void clearLine(int l);
    void dropLines();
    void clearBoard();
};


#endif