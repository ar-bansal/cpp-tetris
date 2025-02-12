#include <iostream>
#include <vector>
#include <thread>
#include <ncurses.h>
#include "board.h"


void pause(int seconds = 0, int milliseconds = 0, int microseconds = 0) {
    if (seconds > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    } else if (milliseconds > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    } else {
        std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
    }
}


Board::Board(int r, int c, char blank) : 
    blankSpace(blank), 
    height(r), 
    width(c), 
    grid(r, std::vector<char>(c, blank)), 
    currPiece(nullptr) {}


void Board::display() {
    clear();  // Clear the screen before drawing

    move(0, 0);
    
    for (int i = 0; i < width + 2; i++) {
        printw("= ");
    }
    printw("\n");

    // Print the board grid
    for (int i = 0; i < height; i++) {
        move(i + 1, 0);  
        printw("| ");

        for (int j = 0; j < width; j++) {

            char toPrint;
            if (currPiece != nullptr) {
                // Check if the cell is inside the bounding box square
                bool x = ((j <= currPiece->location.xmax) && (j >= currPiece->location.xmin));
                bool y = ((i <= currPiece->location.ymax) && (i >= currPiece->location.ymin));
                bool inside = (x && y);
                bool insideAndFilled = false;

                if (inside) {
                    bool filled = (currPiece->shape.grid.at(i - currPiece->location.ymin)
                                .at(j - currPiece->location.xmin) != blankSpace);
                    insideAndFilled = filled && inside;
                }

                if (!insideAndFilled | (currPiece->isFrozen)) {
                    toPrint = grid[i][j];
                } else {
                    toPrint = currPiece->shape.grid.at(i - currPiece->location.ymin)
                                      .at(j - currPiece->location.xmin);
                }
            } else {
                toPrint = grid[i][j];
            }

            printw("%c ", toPrint);  // Print character with spacing
        }

        printw("|");  // Right border
    }

    printw("\n");
    for (int i = 0; i < width + 2; i++) {
        printw("= ");
    }
    printw("\n");

    refresh();  // Refresh the screen to apply changes
}




bool Board::hasSpace() {
    // Check if the middle 4 columns of 
    // the top 4 rows are empty
    for (int i = 0; i < 4; i++) {
        for (int j = (width / 2) - 2; j < (width / 2) + 2; j++) {
            if (grid.at(i).at(j) != blankSpace) {
                return false;
            }
        }
    }
    return true;
}


void Board::insertPiece(Piece& piece) {
    if (hasSpace()) {
        piece.location.xmin = (width - piece.shape.width)  / 2;
        piece.location.xmax = piece.location.xmin + piece.shape.width - 1;

        piece.location.ymin = 0;
        piece.location.ymax = piece.shape.height - 1; 
        
        currPiece = &piece;
    }
}


bool Board::isLineFull(int l) {
    for (int j = 0; j < width; j++) {
        if (grid[l][j] == blankSpace) {
            return false;
        }
    }
    return true;
}


bool Board::isLineEmpty(int l) {
    for (int j = 0; j < width; j++) {
        if (grid[l][j] != blankSpace) {
            return false;
        }
    }
    return true;
}

void Board::clearLine(int l) {
    grid[l] = std::vector<char>(width, blankSpace);
    std::cout << "Cleared line " << l;
}


void Board::dropLines() {
    for (int i = height - 1; i > 0; i--) {
        if (isLineEmpty(i)) {
            auto lineAbove = grid[i-1];
            grid[i] = lineAbove;
            grid[i-1] = std::vector<char>(width, blankSpace);
        }
    }
}


void Board::clearBoard() {
    for (int i = 0; i < height; i++) {
        if (isLineFull(i)) {
            std::cout<< "Line " << i << " is full";
            clearLine(i);
        }
    }

    dropLines();
}
