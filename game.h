#ifndef GAME_H
#define GAME_H

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "fileHandling.h" /*just in case??*/
#include "gameInfo.h"
#define STDIN_FILENO 0

char getUserMove(void); /*function to accept char from the user*/
int getState (int fd); /*takes a fd, returns the state of the game*/

/*helper function for movePlayer, takes the new positions and makes apropriate changes*/
void moveP (char** map, int rows, int cols, int x, int y, GameInfo* gameInfo);

/*simple decides the coordinate of the move, and also if the move should be ignored or not*/
void movePlayer(char move, char** map, int rows, int cols, GameInfo* gameInfo);

/*updtaes the copy file  based off of map array, and gameInfo  */
int updateFile (int fd, char** map, int rows, int cols, GameInfo* gameInfo);

#endif