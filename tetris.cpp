#include <iostream>
#include <thread>
#include <ncurses.h>
#include "pieces.h"
#include "board.h"
#include "input_handler.h"

char blank = '.';
char fill = 'X';
int score(0);
int scoreMultiplier = 200;
int moveDownDelay = 600;
int lastThreshold = 0;      // Score at which the delay was last reduced. 
Board board(10, 10, blank);


void moveDownLoop(Board* board) {
    while (!board->currPiece->isFrozen) {
        board->currPiece->moveDown(*board);
        
        int linesCleared = board->clearBoard();
        score += linesCleared * scoreMultiplier;

        if (score - lastThreshold >= 2 * scoreMultiplier) {
            lastThreshold = score; 
            moveDownDelay /= 1.2;
        }

        board->display();
        std::this_thread::sleep_for(std::chrono::milliseconds(moveDownDelay));
    }
}


int main() {
    InputHandler input_handler;
    input_handler.initInput();

    // Thread for accepting user input.
    std::thread inputThread(&InputHandler::getKeyPress, &input_handler);
    inputThread.detach();

    // Initialize the random piece generator
    PieceGenerator piece_gen(blank, fill, 2463534242u);

    // Initialize the board
    Piece p = piece_gen.getPiece();
    board.insertPiece(p);
    std::thread moveDownThread(moveDownLoop, &board);
    moveDownThread.detach();


    while (true) {
        if (!board.hasSpace()) {
            break;
        } else if (board.currPiece == nullptr) {
            p = piece_gen.getPiece();
            board.insertPiece(p);
        } 

        while (!p.isFrozen) {
            int currInput = input_handler.getCurrInput();
            if ((currInput >= 1000) && (currInput < 1004)) {
                input_handler.blockInput();

                switch (currInput)
                {
                case 1000:
                    // p.rotateLeft();
                    break;
                case 1001:
                    p.rotateRight(board);
                    break;
                case 1002:
                    p.moveLeft(board);
                    break;
                case 1003:
                    p.moveRight(board);
                    break;
                default:
                    break;
                }

                input_handler.resetCurrInput();
            }
            input_handler.allowInput();
        }
    }
    input_handler.closeInput();

    std::cout << "Your score: " << score << '\n';
}
