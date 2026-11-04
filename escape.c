#include "fileHandling.h"
#include "game.h"
#include "gameInfo.h"

int main (int argc, char* argv[]) {

    /*validate command line input*/
    if (argc != 2) {
        write(2, "Usage: ./escape <map.txt>\n", 26);
        return 1;
    }

    /*copy and validate copied fd*/
    int mapFile = copyFile(argv[1]); /*handles opeing and closing of orignal map.txt*/
    if (mapFile < 0) {
        write(2, "copying map.txt failed\n", 23);
        return -1;
    }

    /*get the rows and cols from the copied file*/
    int* res = getRowsAndCols(mapFile);
    int rows = res[0];
    int cols = res[1];
    free(res); /*free the memory allocated for res*/

    /*initialize empty game info*/
    GameInfo gameInfo;

    char** map = loadMap(mapFile, rows, cols, &gameInfo); 
    renderMap(map, rows, cols);
    
    char move;
    char s; 
    while ( gameInfo.state == 0) {
        write(1, "state: ", 7);
        s = singleDigitIntToChar(gameInfo.state);
        write(1, &s, 1);
        write(1, "\n", 1);
        move = getUserMove(); /*get the user input*/
        movePlayer(move, map, rows, cols, &gameInfo);
        
        system("clear");
        system("tput cup 0 0");
        renderMap(map, rows, cols);
    }
    
    /*close the copy file at the very and and then return, do other clean up as well*/
    freeMap(map, rows, cols); /*DEBUG - move to the end later, clean up also needs to be done in case of ^c*/
    close(mapFile);
    return 0;
}
