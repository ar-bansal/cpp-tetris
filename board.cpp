#include <iostream>
#include <vector>
#include "board.h"


Board::Board(int r, int c, char blank) : 
    blankSpace(blank), 
    height(r), 
    width(c), 
    grid(r, std::vector<char>(c, blank)), 
    currPiece(nullptr) {}


void Board::display() {
    std::cout << " | ";
    for (int i = 0; i < width; i++) {
        std::cout << i << ' ';
    }
    std::cout << '|' << '\n';

    for (int i = 0; i < height; i++) {
        std::cout << i << "| ";
        for (int j = 0; j < width; j++) {
            // Check if the cell is inside the bounding box square. 
            bool x = ((j <= currPiece->location.xmax) & (j >= currPiece->location.xmin));
            bool y = ((i <= currPiece->location.ymax) & (i >= currPiece->location.ymin));

            bool inside = (x & y);
            bool insideAndFilled = false;


            if (inside) {
                bool filled = (currPiece->shape.grid.at(i - currPiece->location.ymin).at(j - currPiece->location.xmin) != blankSpace);
                insideAndFilled = filled & inside;
            }

            char toPrint; 
            if (insideAndFilled) {
                toPrint = currPiece->shape.grid.at(i - currPiece->location.ymin).at(j - currPiece->location.xmin);
            } else {
                toPrint = grid[i][j];
            }
            std::cout << toPrint << ' '; 
        }
        std::cout << '|' << '\n';
    }
}


bool Board::hasSpace(const Piece& piece) {
    for (int i = 0; i < piece.shape.height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] != blankSpace) {
                return false;
            }
        }
    }
    return true;
}


void Board::insertPiece(Piece& piece) {
    if (hasSpace(piece)) {
        piece.location.xmin = (width - piece.shape.width)  / 2;
        piece.location.xmax = piece.location.xmin + piece.shape.width - 1;

        piece.location.ymin = 0;
        piece.location.ymax = piece.shape.height - 1; 
        
        currPiece = &piece;
    }
}