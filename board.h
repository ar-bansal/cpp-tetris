
#ifndef BOARD_H
#define BOARD_H

#include "pieces.h"
#include <vector>


class Piece;

class Board {
private:

public: 
    std::vector<std::vector<char>> grid;
    
    int width;
    int height;
    char blankSpace;
    
    Piece* currPiece;

    Board(int r, int c, char blank);

    void display();
    void displayLoop();
    bool hasSpace(); 
    void insertPiece(Piece& piece);

    bool isLineFull(int l);
    bool isLineEmpty(int l);
    void clearLine(int l);
    void dropLines();
    int clearBoard();
};


#endif