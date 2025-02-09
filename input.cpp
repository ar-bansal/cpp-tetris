#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ncurses.h>
#include <thread>
#include <chrono>
#include "tetrominos.h"


void clearScreen() {
    std::cout << "\033[2J\033[H";
    std::cout.flush();
}


void transposeAntidiagonal(int xmin, int xmax, int ymin, int ymax, std::vector<std::vector<char>>& shape) {
    for (int i = 0; i <= ymax - ymin; i++) {
        for (int j = 0; j <= i; j++) {
            std::swap(shape[i+ymin][j+xmin], shape[j+ymin][i+xmin]);
        }
    }
}

void transposeDiagonal(int xmin, int xmax, int ymin, int ymax, std::vector<std::vector<char>>& shape) {
    int width = xmax - xmin;

    for (int i = ymax - ymin; i >= 0; i--) {
        for (int j = i; j >= 0; j--) {
            std::swap(shape[width-i+ymin][j+xmin], shape[width-j+ymin][i+xmin]);
        }
    }

}

void flip(int xmin, int xmax, int ymin, int ymax, std::vector<std::vector<char>>& shape) {
    for (int i = ymin; i <= ymax; i++) {
        for (int j = xmin; j <= (xmin + xmax) / 2; j++) {
            std::swap(shape[i][j], shape[i][xmax + xmin - j]);
        }
    }
}


class Grid
{
private:
public:
    std::vector<std::vector<char>> board;
    int rows;
    int cols;
    char blankSpace;
    std::vector<int> currShapeCoords;
    
    Grid(int r, int c, char blank) : rows(r), cols(c), blankSpace(blank), board(r, std::vector<char>(c, blank)) {}

    void display(bool clear = false) {
        if (clear) {
            clearScreen();
        }


        for (int i = rows - 1; i >= 0; i--) {
            std::cout << i << "| ";
            for (int j = 0; j < cols; j++) {
                // std::cout << board[i][j]; // << ' ';
                std::cout << board[i][j] << ' ';
            }
            
            std::cout << '|' << '\n';
        }

        std::cout << " | ";
        for (int i = 0; i < cols; i++) {
            // std::cout << i; // << ' ';
            std::cout << i << ' ';
        }
        std::cout << '|' << '\n';
    }

    void displayInv() {
        for (int i = 0; i < rows; i++) {
            std::cout << i << "| ";
            for (int j = 0; j < cols; j++) {
                std::cout << board[i][j] << ' ';
            }
            
            std::cout << '|' << '\n';
        }
    }

    bool insertNewShape(auto& shape) {
        // Check if the top 4 rows are empty. Can be changed to a more sophisticated logic
        for (int i = rows - 1; i > rows - 1 - 4; i--) {
            for (int j = 0; j < cols - 1; j++) {
                if (board[i][j] == blankSpace) {
                    continue;
                } else {
                    currShapeCoords = {0, 0, 0, 0};
                    return false;
                }
            }
        }

        int center = cols / 2;

        int shapeCenter = shape.width / 2;  

        int xmin = center - shapeCenter;
        int xmax = xmin + shape.width - 1;

        int ymax = (rows - 1);
        int ymin = ymax - (shape.height - 1);

        // Center the shape on the grid and insert it
        for (int i = rows - 1; i > ymax - shape.height; i--) {
            for (int j = xmin; j < center + shape.width - 1; j++) {
                board[i][j] = shape.miniGrid[ymax - i][j - xmin];
            }
        }

        currShapeCoords = {xmin, xmax, ymin, ymax};

        return true;
    }

    
    bool moveDown() {
        int xmin = currShapeCoords[0];
        int xmax = currShapeCoords[1];
        int ymin = currShapeCoords[2];
        int ymax = currShapeCoords[3];


        // Check if the space below is available for all non blankSpace 
        // squares in the current shape's last layer.
        bool allAvailable = true;
        for (int p = xmin; p <= xmax; p++) {
            if (ymin >= 1) {
                bool currBlank = (board[ymin][p] == blankSpace);
                bool nextBlank = ((ymin >= 1) & (board[ymin-1][p] == blankSpace));
                allAvailable = (allAvailable & (currBlank | (!currBlank & nextBlank)));
            } else {
                allAvailable = false;
            }
            

            // std::cout << currBlank << ' ' << nextBlank << ' ' << allAvailable << '\n';
        }

        if (!allAvailable) {
            return false;
        }
        std::cout << "down available" << '\n';

        for (int i = std::max(ymin, 1); i <= ymax; i++) {
            for (int j = xmin; j <= xmax; j++) {
                char& curr = board[i][j];
                char& below = board[i-1][j];
                std::swap(curr, below);
            }
        }

        currShapeCoords[2] -= 1;
        currShapeCoords[3] -= 1;

        return true;
    }

    bool moveRight() {
        int xmin = currShapeCoords[0];
        int xmax = currShapeCoords[1];
        int ymin = currShapeCoords[2];
        int ymax = currShapeCoords[3];

        // Check if the space on the right is available for all non blankSpace
        // squares in the current shape's right col
        bool allAvailable = true;
        for (int p = ymin; p <= ymax; p++) {
            bool currBlank = (board[p][xmax] == blankSpace);
            // Check if the current shape is not at the right extreme 
            // and if there is space available
            bool rightBlank = ((xmax < cols - 1) & (board[p][xmax+1] == blankSpace));
            allAvailable = (allAvailable & (currBlank | rightBlank));

            std::cout << std::boolalpha;
            std::cout << currBlank << ' ' << rightBlank << ' ' << allAvailable << '\n';

        }

        if (!allAvailable) {
            return false;
        } else {
            for (int i = ymin; i <= ymax; i++) {
                for (int j = xmax; j >= xmin; j--) {
                    // std::swap(board[i][j], board[i][j+1]);
                    board[i][j+1] = board[i][j];
                }
                board[i][xmin] = blankSpace;
            }
        }

        currShapeCoords[0] += 1;
        // currShapeCoords[1] += 1;
        currShapeCoords[1] = std::min(currShapeCoords[1] + 1, cols - 1);
        std::cout << "moved right" << '\n';

        return true;
    }


    // bool moveLeft() {}

    void rotateRight() {
        int xmin = currShapeCoords[0];
        int xmax = currShapeCoords[1];
        int ymin = currShapeCoords[2];
        int ymax = currShapeCoords[3];
        transposeDiagonal(xmin, xmax, ymin, ymax, board);
        flip(xmin, xmax, ymin, ymax, board);
    }

    void rotateLeft() {
        int xmin = currShapeCoords[0];
        int xmax = currShapeCoords[1];
        int ymin = currShapeCoords[2];
        int ymax = currShapeCoords[3];
        transposeAntidiagonal(xmin, xmax, ymin, ymax, board);
        flip(xmin, xmax, ymin, ymax, board);
    }
};


void display(std::vector<std::vector<char>>& shape) {
    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[1].size(); j++) {
            std::cout << shape[i][j] << ' ';
        }
        std::cout << '\n';
    }
}


void printVector(const std::vector<int>& vec) {
    std::cout << "[ ";
    for (const int& val : vec) {
        std::cout << val << " ";
    }
    std::cout << "]\n";
}

int main() {
    int nrows = 10;
    int ncols = 8;
    char blankSpace = '.';
    Grid g(nrows, ncols, blankSpace);

    ShapeL shape(blankSpace);


    // g.display();
    // clearScreen();

    std::cout << '\n';
    bool inserted = g.insertNewShape(shape);
    g.display();
    g.rotateRight();
    g.rotateRight(); 
    // transposeDiagonal()
    // std::vector<int> currShapeCoords = g.currShapeCoords;
    // int xmin = currShapeCoords[0];
    // int xmax = currShapeCoords[1];
    // int ymin = currShapeCoords[2];
    // int ymax = currShapeCoords[3];
    // transposeDiagonal(xmin, xmax, ymin, ymax, g.board);
    // g.display();

    // for (int i = 0; i < 20; i++ ) {
    //     g.rotateRight();
    //     printVector(g.currShapeCoords);
    //     g.display();
    //     std::this_thread::sleep_for(std::chrono::seconds(1));

    //     bool m;
    //     m = g.moveDown();
    //     // std::cout << '\n';
    //     // g.rotateLeft();
    //     // g.display();
    //     // std::this_thread::sleep_for(std::chrono::seconds(1));
    //     if (!m) {
    //         break;
    //     }
    // }

    int i = 0;
    while (inserted) {
        std::cout << "here" << '\n';
        bool moved = g.moveDown();

        if (i % 1 == 0) {
            std::cout << "moving right" << '\n';
            g.moveRight();
        }
        // if (i % 3 == 0) {
        //     g.rotateRight();
        // }
        printVector(g.currShapeCoords);
        g.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        if (!moved) {
            ShapeL shape(blankSpace);
            inserted = g.insertNewShape(shape);
            g.rotateRight();
            g.rotateRight();
        }
        i += 1;
    }
    std::cout << "Could not insert another block" << '\n';
}
