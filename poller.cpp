#include <ncurses.h>
#include <iostream>
#include <unistd.h>


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

// Function to get a single key press (returns -1 if no key is pressed)
// int getKeyPress() {
//     int ch = getch();  // Read input (non-blocking)
//     // return (ch != ERR) ? ch : -1;  // Return key if pressed, else -1

//     switch (ch) {
//         case KEY_UP:     return 1000;
//         case KEY_DOWN:   return 1001;
//         case KEY_LEFT:   return 1002;
//         case KEY_RIGHT:  return 1003;
//         default:         return ch;
//     }
// }