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