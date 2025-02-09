#include <iostream>
#include <thread>
#include "pieces.h"
#include "board.h"


int main() {
    char blank = '.';
    Board board(10, 10, blank);
    LPiece l(blank);

    board.insertPiece(l);
    std::cout << l.location.xmin << l.location.xmax << l.location.ymin << l.location.ymax << '\n';

    board.display(l);
    std::cout << '\n';

    for (int i = 0; i < 20; i++) {
        // l.moveLeft(board);
        // std::cout << '\n';
        // board.display(l);
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // if (i % 2 == 0) {
        //     l.moveRight(board);
        //     board.display(l);
        //     std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // }
        l.moveDown(board);
        board.display(l);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        if (l.isFrozen) {
            std::cout << "frozen" << '\n';
            l = LPiece(blank);
            board.insertPiece(l);
        }

        if (i % 2 == 0) {
            l.moveRight(board);
            board.display(l);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        if (i % 3 == 0) {
            l.rotateRight();
        }
    }
}