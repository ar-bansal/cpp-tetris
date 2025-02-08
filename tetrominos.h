#ifndef TETROMINOS_H
#define TETROMINOS_H

#include <vector>

class ShapeL {
public:
    std::vector<std::vector<char>> miniGrid;
    int height;
    int width;
    char blankSpace;
    std::vector<int> coords;

    ShapeL(char blank);
};

#endif