#include "input_handler.h"


InputHandler::InputHandler() {
    currInput = 0;
    inputAllowed = false;
}


void InputHandler::initInput() {
    initscr();          // Start ncurses mode
    cbreak();           // Disable line buffering (no need for Enter)
    noecho();           // Don't show typed characters
    nodelay(stdscr, TRUE);  // Make getch() non-blocking
    keypad(stdscr, TRUE);   // Enable special keys (arrows, function keys)
}


void InputHandler::closeInput() {
    endwin();
}


void InputHandler::getKeyPress() {
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


int InputHandler::getCurrInput() {
    return currInput;
}


void InputHandler::resetCurrInput() {
    currInput = 0;
}


void InputHandler::blockInput() {
    inputAllowed = false;
}


void InputHandler::allowInput() {
    inputAllowed = true;
}