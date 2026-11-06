#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h> /*for malloc and free*/
#include "game.h"
#include "gameInfo.h"


/*------------------------------------- HELPERS ---------------------------------------*/
int stringLen(char* str); /*returns the length of a string, not including the null terminator*/
char singleDigitIntToChar(int num); /*converts a single digit int to a char*/
int singleDigitCharToInt(char ch); /*converts a single digit char to an int*/
char* twoDigitIntToChar(int num); /*takes a 2 digit int, returns a char* representation of it*/
int twoDigitCharToInt(char* str); /*takes a 2 digit char*, returns the int representation of it*/
void setWhiteBackground();
void resetBackground();
void clear_terminal();

/*--------------------------------- MAIN OPERATIONS -----------------------------------*/
int* getRowsAndCols(int fd); /*takes a fd, returns an int* of size 2 with rows and cols*/
int copyFile (char* file); /*takes fd, creates a copy, return the copied fd*/
void loadMap(char** map, int fd, int rows, int cols, GameInfo* gameInfo); /*takes an empty matrix, a fd, reads map contents, returns 2d array representaion*/
int renderMap (char** map, int rows, int cols); /*takes a 2d array map, renders it out to the terminal*/
int freeMap (char** map, int rows);  /*takes a 2d array map, and then frees it */

#endif