#include"terminal.h"


void disableBuffer(){
    struct termios mode;

    tcgetattr(0, &mode);
    mode.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(0, TCSANOW, &mode);
}

void enableBuffer(){
    struct termios mode;

    tcgetattr(0, &mode);
    mode.c_lflag |= (ECHO | ICANON);
    tcsetattr(0, TCSANOW, &mode);
}

/*function to accept char from the user*/
char getUserMove(void) {
    char move;
    read(STDIN_FILENO, &move, 1); /*read a single char from stdin*/
    return move;
}
