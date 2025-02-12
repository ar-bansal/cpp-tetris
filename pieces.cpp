#include <iostream>
#include <vector>
#include "pieces.h"


Piece::Piece(char blank) : blankSpace(blank), isFrozen(false) {}


void Piece::transposeDiagonal() {
    for (int i = 0; i < shape.height; i++) {
        for (int j = 0; j < i; j++) {
            std::swap(shape.grid[i][j], shape.grid[j][i]);
        }
    }
}

void Piece::transposeAntidiagonal() {
    for (int i = 0; i < shape.height; i++) {
        for (int j = 0; j < i; j++) {
            std::swap(shape.grid[shape.width-i][j], shape.grid[shape.width-j][i]);
        }
    }
}

void Piece::flip() {
    for (int i = 0; i < shape.height; i++) {
        for (int j = 0; j < shape.width / 2; j++) {
            std::swap(shape.grid[i][j], shape.grid[i][shape.width - 1 - j]);
        }
    }
}


bool Piece::isRotationValid(Board& board) {
    for (int i = location.ymin; i <= location.ymax; i++) {
        for (int j = location.xmin; j <= location.xmax; j++) {
            // Check if inside the board limits
            bool x = ((j < board.width) && (j >= 0));
            bool y = ((i < board.height) && (i >= 0));

            if (!x | !y) {
                return false;
            }

            std::cout << "Checking here" << '\n';

            // Check if rotation will overlap with an occupied cell on the board
            char currCell = shape.grid.at(i - location.ymin).at(j - location.xmin);
            if ((currCell != blankSpace) && (board.grid.at(i).at(j) != board.blankSpace)) {
                return false;
            }
        }
    }

    return true;
}

void Piece::rotateRight(Board& board) {
    if (!isFrozen) {
        transposeDiagonal();
        flip();

        std::cout << "rotated right" << '\n';
        std::cout << location.xmin << ' ' << location.xmax << ' ' << location.ymin << ' ' << location.ymax << '\n';
        if (!isRotationValid(board)) {
            flip();
            transposeDiagonal();
        }
    }
    board.display();
};


void Piece::rotateLeft(Board& board) {
    if (!isFrozen) {
        transposeAntidiagonal();
        flip();

        if (!isRotationValid(board)) {
            flip();
            transposeAntidiagonal();
        }
    }
    board.display();
}


bool Piece::isValidMoveRight(Board& board) {
    for (int i = location.ymin; i <= location.ymax; i++) {
        for (int j = location.xmin; j <= location.xmax; j++) {
            // Only check for non-empty cells in the shape
            if (shape.grid[i - location.ymin][j - location.xmin] != blankSpace) {
                // Check if moving the cell right would go out of bounds
                if (j + 1 >= board.width) {
                    return false;
                }

                // Check if the corresponding cell to the right is occupied on the board
                if (board.grid[i][j + 1] != blankSpace) {
                    return false;
                }

            }
        }
    }
    return true;
}


void Piece::moveRight(Board& board) {
    if (isValidMoveRight(board)) {
        location.xmax += 1;
        location.xmin += 1;
        board.display();
    }
}


bool Piece::isValidMoveLeft(Board& board) {
    for (int i = location.ymin; i <= location.ymax; i++) {
        for (int j = location.xmax; j >= location.xmin; j--) {
            // Only check for non-empty cells in the shape
            if (shape.grid[i - location.ymin][j - location.xmin] != blankSpace) {
                // Check if moving left will go out of bounds
                if (j - 1 < 0) {
                    return false;
                }

                // Check if the corresponding cell to the left is occupied on the board
                if (board.grid[i][j-1] != blankSpace) {
                    return false;
                }
            }
        }
    }
    return true;
}


void Piece::moveLeft(Board& board) {
    if (isValidMoveLeft(board)) {
        board.displayMutex.lock();
        location.xmax -= 1;
        location.xmin -= 1;
        board.displayMutex.unlock();
        board.display();
    }
}


bool Piece::isValidMoveDown(Board& board) {
    for (int i = location.ymax; i >= location.ymin; i--) {
        for (int j = location.xmin; j <= location.xmax; j++) {
            if (shape.grid[i - location.ymin][j - location.xmin] != blankSpace) {
                // Check for out of bounds movement
                if (i + 1 >= board.height) {
                    return false;
                }

                // Check if the space below is occupied on the board
                if (board.grid[i+1][j] != blankSpace) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Piece::freeze(Board& board) {
    for (int i = 0; i < board.height; i++) {
        for (int j = 0; j < board.width; j++) {
            // Check if the cell is inside the bounding box square. 
            bool x = ((j <= location.xmax) && (j >= location.xmin));
            bool y = ((i <= location.ymax) && (i >= location.ymin));

            bool inside = (x && y);
            bool insideAndFilled = false;

            if (inside) {
                bool filled = (shape.grid.at(i - location.ymin).at(j - location.xmin) != blankSpace);
                insideAndFilled = filled && inside;
            }

            if (insideAndFilled) {
                board.grid[i][j] = shape.grid.at(i - location.ymin).at(j - location.xmin);
            }
        }
    }

    // location.xmin = -1;
    // location.xmax = -1;
    // location.ymin = -1;
    // location.ymax = -1;
    location = {-1, -1, -1, -1};
    board.currPiece = nullptr;
    isFrozen = true;
}

void Piece::moveDown(Board& board) {
    if (isValidMoveDown(board)) {
        location.ymax += 1;
        location.ymin += 1;
    } else if (!isFrozen) {
        freeze(board);
    }
    // board.display();
}



IPiece::IPiece(char blank, char fill) : Piece(blank) {
    shape.grid = {
        {blank, fill, blank, blank}, 
        {blank, fill, blank, blank}, 
        {blank, fill, blank, blank}, 
        {blank, fill, blank, blank}
    };

    shape.height = shape.grid.size();
    shape.width = shape.grid.at(0).size();
}


JPiece::JPiece(char blank, char fill) : Piece(blank) {
    shape.grid = {
        {blank, blank, fill, blank}, 
        {blank, blank, fill, blank}, 
        {blank, fill, fill, blank}, 
        {blank, blank, blank, blank}
    };

    shape.height = shape.grid.size();
    shape.width = shape.grid.at(0).size();
}


LPiece::LPiece(char blank, char fill) : Piece(blank) {
    shape.grid = {
        {blank, fill, blank, blank}, 
        {blank, fill, blank, blank}, 
        {blank, fill, fill, blank}, 
        {blank, blank, blank, blank}
    };

    shape.height = shape.grid.size();
    shape.width = shape.grid.at(0).size();
}


OPiece::OPiece(char blank, char fill) : Piece(blank) {
    shape.grid = {
        {blank, fill, fill, blank}, 
        {blank, fill, fill, blank}, 
        {blank, blank, blank, blank}, 
        {blank, blank, blank, blank}
    };

    shape.height = shape.grid.size();
    shape.width = shape.grid.at(0).size();
}


SPiece::SPiece(char blank, char fill) : Piece(blank) {
    shape.grid = {
        {blank, fill, fill, blank}, 
        {fill, fill, blank, blank}, 
        {blank, blank, blank, blank}, 
        {blank, blank, blank, blank}
    };

    shape.height = shape.grid.size();
    shape.width = shape.grid.at(0).size();
}


TPiece::TPiece(char blank, char fill) : Piece(blank) {
    shape.grid = {
        {fill, fill, fill, blank}, 
        {blank, fill, blank, blank}, 
        {blank, blank, blank, blank}, 
        {blank, blank, blank, blank}
    };

    shape.height = shape.grid.size();
    shape.width = shape.grid.at(0).size();
}


ZPiece::ZPiece(char blank, char fill) : Piece(blank) {
    shape.grid = {
        {fill, fill, blank, blank}, 
        {blank, fill, fill, blank}, 
        {blank, blank, blank, blank}, 
        {blank, blank, blank, blank}
    };

    shape.height = shape.grid.size();
    shape.width = shape.grid.at(0).size();
}