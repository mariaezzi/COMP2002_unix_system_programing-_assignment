#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#define STDIN_FILENO 0

/*function to accept char from the user*/
char getUserMove(void);