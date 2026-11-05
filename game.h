#ifndef GAME_H
#define GAME_H

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "fileHandling.h"
#include "gameInfo.h"
#include "random.h"

#define STDIN_FILENO 0

/*function to accept char from the user*/
char getUserMove(void); 

/*takes the fd of copied map file, returns the current state of the game*/
int getState (int fd); 

/*updtaes the copy file  based off of map array, and gameInfo  */
int updateFile (int fd, char** map, int rows, int cols, GameInfo* gameInfo);



/* ~~~~~~~~~~~~~~~ changes file and struct (critical section) ~~~~~~~~~~~~~~~*/

/*helper function for movePlayer, takes the new positions and makes apropriate changes*/
void moveP (char** map, int rows, int cols, int x, int y, GameInfo* gameInfo, int fd);

/*takes new x and y coordinates for snake, and updtaes relevant info if valid move*/
void moveS (char** map, int rows, int cols, int x, int y, GameInfo* gameInfo, int fd);

/*takes new x and y coordinates for Wolf, and updtaes relevant info if valid move*/
void moveW (char** map, int rows, int cols, int x, int y, GameInfo* gameInfo, int fd);

/*~~~~~~~~~~~~~~~~~~~~~~ Returns coordinates of valid move (doesnt make changes tho) ~~~~~~~~~~~~~~~~~~~~~~*/

/*simple decides the coordinate of the move, and also if the move should be ignored or not*/
void movePlayer(char move, char** map, int rows, int cols, GameInfo* gameInfo, int fd);

/*move randomly one step in any availabe 8 directions, move every 2.5 sec*/
/*helper function for moveS*/
void moveSnake (char** map, int rows, int cols, GameInfo* gameInfo, int fd);

/*move randomly one step in any availabe 4 directions, move every 1 sec*/
/*gets the exact new coordinates, then passes to helper that actually makes the move*/
void moveWolf (char** map, int rows, int cols, GameInfo* gameInfo, int fd);



#endif