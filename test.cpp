#include <iostream>
#include <thread>
#include <ncurses.h>
#include <unistd.h>
#include <mutex>
#include "pieces.h"
#include "board.h"


std::mutex inputMutex;

static int currInput = 0;
static bool inputAllowed = true;

void initInput() {
    initscr();          // Start ncurses mode
    cbreak();           // Disable line buffering (no need for Enter)
    noecho();           // Don't show typed characters
    nodelay(stdscr, TRUE);  // Make getch() non-blocking
    keypad(stdscr, TRUE);   // Enable special keys (arrows, function keys)
}

void closeInput() {
    endwin();  // Restore normal terminal behavior
}

void getKeyPress() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));

        if ((currInput == 0) && inputAllowed) {
            int ch = getch(); 
            flushinp();  // Prevent extra key presses from accumulating
        
            if (ch != ERR) {
                switch (ch) {
                case KEY_UP:     
                    currInput = 1000;
                    break;
                case KEY_DOWN:   
                    currInput = 1001;
                    break;
                case KEY_LEFT:   
                    currInput = 1002;
                    break;
                case KEY_RIGHT:  
                    currInput = 1003;
                    break;
                default:         
                    currInput = 0;
                    break;
                }
            }
        }
    }
}


void moveDownLoop(Board* board) {
    // while (threadBool.load()) {
    while (!board->currPiece->isFrozen) {
        board->currPiece->moveDown(*board);
        board->clearBoard();
        board->display();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // refresh();
    }
    printw("Thread exiting");
}


char blank = '.';
char fill = 'X';
int score(0);
Board board(20, 10, blank);

int main() {
    initInput();

    // Thread for accepting user input.
    std::thread inputThread(getKeyPress);
    inputThread.detach();

    // Initialize the board
    JPiece p(blank, fill);
    board.insertPiece(p);
    std::thread moveDownThread(moveDownLoop, &board);
    moveDownThread.detach();


    while (true) {
        if (!board.hasSpace()) {
            break;
        } else if (board.currPiece == nullptr) {
            // get piece function
            p = JPiece(blank, fill);
            board.insertPiece(p);
        } 

        while (!p.isFrozen) {
            if ((currInput >= 1000) && (currInput < 1004)) {
                inputAllowed = false;

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

                currInput = 0;
            }
            inputAllowed = true;
        }
    }

    closeInput();
}
