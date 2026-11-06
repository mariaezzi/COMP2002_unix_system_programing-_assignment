#ifndef TERMINAL_H
#define TERMINAL_H

#include <unistd.h>
#include<termios.h>
#define STDIN_FILENO 0

void disableBuffer();
void enableBuffer();
char getUserMove();

#endif