#include <iostream>
#include <thread>
#include <ncurses.h>
#include <unistd.h>
#include <mutex>
#include "pieces.h"
#include "board.h"
// #include "poller.cpp"


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
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 48));

        if ((currInput == 0) & inputAllowed) {
            // if (inputMutex.try_lock()) {

            int ch = getch();  // Read input (non-blocking)
            flushinp();
            // return (ch != ERR) ? ch : -1;  // Return key if pressed, else -1

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
                // inputMutex.unlock();
                // refresh();
                // printw("New key press: %i", currInput);
        }
    }
}

char blank = '.';
char fill = 'X';
Board board(20, 10, blank);


void runGameLoop() {
    JPiece p(blank, fill);
    while (true) {
        if (board.hasSpace()) {
            p = JPiece(blank, fill); 
            p.rotateRight(board);
            board.insertPiece(p);
        } else {
            break;
        }

        // i++;
        while (!p.isFrozen) {
            if ((currInput >= 1000) & (currInput < 1004)) {
                inputAllowed = false;
                // inputMutex.lock();

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
                // inputMutex.unlock();

                currInput = 0;
            }
            inputAllowed = true;
            p.moveDown(board);
            // std::cout << p.location.xmin << ' ' << p.location.xmax << ' ' << p.location.ymin << ' ' << p.location.ymax << '\n';

            board.clearBoard();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
    }
}


int main() {
    initInput();
    std::thread inputThread(getKeyPress);
    std::thread gameLoopThread(runGameLoop);
    inputThread.detach();
    gameLoopThread.join();

    closeInput();


}







// int main() {
//     int i = 1;
    
//     JPiece p(blank, fill);
//     while (true) {
//         if (board.hasSpace()) {
//             p = JPiece(blank, fill); 
//             p.rotateRight(board);
//             board.insertPiece(p);
//         } else {
//             break;
//         }

//         i++;
//         while (!p.isFrozen) {
//             if (i % 2 == 0) {
//                 p.rotateRight(board);
//                 p.moveRight(board);
//                 std::cout << p.location.xmin << ' ' << p.location.xmax << ' ' << p.location.ymin << ' ' << p.location.ymax << '\n';
                
//             } 
//             if (i % 3 == 0) {
//                 p.moveLeft(board);
//                 p.moveLeft(board);
//                 std::cout << p.location.xmin << ' ' << p.location.xmax << ' ' << p.location.ymin << ' ' << p.location.ymax << '\n';
//             }
//             p.moveDown(board);
//             std::cout << p.location.xmin << ' ' << p.location.xmax << ' ' << p.location.ymin << ' ' << p.location.ymax << '\n';

//             board.clearBoard();
//             std::this_thread::sleep_for(std::chrono::milliseconds(350));
//         }
//     }

// }
