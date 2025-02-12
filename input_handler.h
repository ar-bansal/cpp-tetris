#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <ncurses.h>
#include <thread>


class InputHandler {
private:
    int currInput;
    bool inputAllowed;
public: 
    InputHandler();

    void initInput();
    void closeInput();

    void getKeyPress();

    int getCurrInput();
    void resetCurrInput();

    void blockInput();
    void allowInput();
};


#endif


// static int currInput = 0;
// static bool inputAllowed = true;

// void initInput() {
//     initscr();          // Start ncurses mode
//     cbreak();           // Disable line buffering (no need for Enter)
//     noecho();           // Don't show typed characters
//     nodelay(stdscr, TRUE);  // Make getch() non-blocking
//     keypad(stdscr, TRUE);   // Enable special keys (arrows, function keys)
// }

// void closeInput() {
//     endwin();  // Restore normal terminal behavior
// }

// void getKeyPress() {
//     while (true) {
//         std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));

//         if ((currInput == 0) && inputAllowed) {
//             int ch = getch(); 
//             flushinp();  // Prevent extra key presses from accumulating
        
//             if (ch != ERR) {
//                 switch (ch) {
//                 case KEY_UP:     
//                     currInput = 1000;
//                     break;
//                 case KEY_DOWN:   
//                     currInput = 1001;
//                     break;
//                 case KEY_LEFT:   
//                     currInput = 1002;
//                     break;
//                 case KEY_RIGHT:  
//                     currInput = 1003;
//                     break;
//                 default:         
//                     currInput = 0;
//                     break;
//                 }
//             }
//         }
//     }
// }