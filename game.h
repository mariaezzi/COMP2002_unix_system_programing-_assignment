#ifndef GAME_H
#define GAME_H

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "fileHandling.h"
#include "gameInfo.h"
#include "random.h"

#define STDIN_FILENO 0

char getUserMove(void); /*function to accept char from the user*/
int getState (int fd); /*takes a fd, returns the state of the game*/

/*helper function for movePlayer, takes the new positions and makes apropriate changes*/
void moveP (char** map, int rows, int cols, int x, int y, GameInfo* gameInfo, int fd);

/*simple decides the coordinate of the move, and also if the move should be ignored or not*/
void movePlayer(char move, char** map, int rows, int cols, GameInfo* gameInfo, int fd);

/*takes new x and y coordinates for snake, and updtaes relevant info if valid move*/
void moveS (char** map, int rows, int cols, int x, int y, GameInfo* gameInfo, int fd);

/*move randomly one step in any availabe 8 directions, move every 2.5 sec*/
/*helper function for moveS*/
void moveSnake (char** map, int rows, int cols, GameInfo* gameInfo, int fd);

/*updtaes the copy file  based off of map array, and gameInfo  */
int updateFile (int fd, char** map, int rows, int cols, GameInfo* gameInfo);

#endif