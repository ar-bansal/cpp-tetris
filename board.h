
#ifndef BOARD_H
#define BOARD_H

#include "pieces.h"
#include <vector>
#include <optional>
#include <mutex>

class Piece;

class Board {
private:

public: 
    std::mutex displayMutex;
    std::vector<std::vector<char>> grid;
    
    int width;
    int height;
    char blankSpace;
    
    Piece* currPiece;

    Board(int r, int c, char blank);

    // void display(const Piece& piece);
    void display();
    void displayLoop();
    bool hasSpace(); 
    // bool hasSpace(); 
    void insertPiece(Piece& piece);

    bool isLineFull(int l);
    bool isLineEmpty(int l);
    void clearLine(int l);
    void dropLines();
    void clearBoard();
};


#endif