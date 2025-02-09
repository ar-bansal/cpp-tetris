#include <iostream>
#include <thread>
#include "pieces.h"
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


int main() {
    char blank = '.';
    char fill = 'X';
    Board board(10, 4, blank);
    // LPiece l(blank, fill);
    IPiece l(blank, fill);

    l.rotateRight();
    board.insertPiece(l);
    // std::cout << l.location.xmin << l.location.xmax << l.location.ymin << l.location.ymax << '\n';

    board.display();
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
        // l.moveDown(board);
        // board.display();
        // pause(0, 500);

        // if (l.isFrozen) {
        //     board.isLineFull();
        //     std::cout << "frozen" << '\n';
        //     l = LPiece(blank, fill);
        //     board.insertPiece(l);
        // }

        // if (i % 2 == 0) {
        //     l.moveRight(board);
        //     board.display();
        //     pause(0, 500);
        // }

        // if (i % 3 == 0) {
        //     l.rotateRight();
        // }

        pause(0, 600);
        l.moveDown(board);
        std::cout << "here1" << '\n';
        board.display();
        if (l.isFrozen) {
            board.clearBoard();
            board.display();
            std::cout << "here2" << '\n';
        }
    }
}