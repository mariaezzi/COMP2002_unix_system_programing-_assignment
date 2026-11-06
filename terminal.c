#include"terminal.h"


void disableBuffer(){
    struct termios mode;

    tcgetattr(0, &mode);
    mode.c_lflag &= ~(ECHO | ICANON);

    /*give up waiting after about 10th of a second - this is to fix the lag in the lose condition*/
    mode.c_cc[VMIN] = 0;
    mode.c_cc[VTIME] = 1;

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
    int res = read(STDIN_FILENO, &move, 1); /*read a single char from stdin*/
    if (res <= 0) {
        return '\0';
    }
    return move;
}
