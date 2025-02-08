#include <vector>
#include "tetrominos.h"


ShapeL::ShapeL(char blank) : blankSpace(blank), miniGrid{
    {blank, 'X', blank},
    {blank, 'X', blank},
    {blank, 'X', 'X'}
}  {
    height = miniGrid.size();
    width = miniGrid[0].size();
}






// class ShapeL {
// public:
//     std::vector<std::vector<char>> miniGrid;

//     ShapeL() : miniGrid{
//         {0, 0, 0, 0}, 
//         {1, 0, 0, 0}, 
//         {1, 0, 0, 0}, 
//         {1, 1, 0, 0}, 
//     } {}
// };


class ShapeJ {
public:
    std::vector<std::vector<char>> miniGrid;

    ShapeJ() : miniGrid{
        {0, 0, 0, 0}, 
        {0, 0, 0, 1}, 
        {0, 0, 0, 1}, 
        {0, 0, 1, 1}, 
    } {}
};


class ShapeO {
public:
    std::vector<std::vector<char>> miniGrid;

    ShapeO() : miniGrid{
        {0, 0, 0, 0}, 
        {0, 0, 0, 0}, 
        {0, 1, 1, 0}, 
        {0, 1, 1, 0}, 
    } {}
};
