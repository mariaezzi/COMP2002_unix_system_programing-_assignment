#include "game.h"

/*function to accept char from the user*/
char getUserMove(void) {
    struct termios old, new;

    /*get the current terminal settings*/
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO); /*disable canonical mode and echo*/
    
    /*set the new terminal settings*/
    tcsetattr(STDIN_FILENO, TCSANOW, &new);


    /*get the user input*/
    char move;
    read(STDIN_FILENO, &move, 1); /*read a single char from stdin*/

    /*restore orignal terminal*/
    tcsetattr(STDIN_FILENO, TCSANOW, &old);

    return move;
}