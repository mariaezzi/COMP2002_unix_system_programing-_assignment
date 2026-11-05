#include "fileHandling.h"
#include "game.h"
#include "gameInfo.h"
#include "newSleep.h"

/*multi proccess includes*/
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/file.h>

int main (int argc, char* argv[]) {

    /*set the random seed*/
    initRandom();

    /*validate command line input*/
    if (argc != 2) {
        write(2, "Usage: ./escape <map.txt>\n", 26);
        return 1;
    }

    /*copy orignal map and validate copied fd*/
    int mapFile = copyFile(argv[1]); /*handles opeing and closing of orignal map.txt*/
    if (mapFile < 0) {
        write(2, "copying map.txt failed\n", 23);
        return -1;
    }

    /*get the rows and cols from the copied file, doing this in main, make parameter passing easier*/
    int* res = getRowsAndCols(mapFile);
    int rows = res[0];
    int cols = res[1];
    free(res); /*free the memory allocated for res*/

    /*initialize empty game info*/
    GameInfo gameInfo;

    /*initialize 2d array*/
    char** map = (char**)malloc(rows * sizeof(char*));
    if (map == NULL) {
        write(2, "Map Memory allocation failed\n", 29);
        return -1;
    }
    int i;
    for (i = 0; i < rows; i ++) {
        map[i] = (char*)malloc(cols * sizeof(char));
    }

    /*load gama data into array and struct*/
    loadMap(map , mapFile, rows, cols, &gameInfo); 


    /*------------------ MAIN GAME LOOP ----------------------------*/

    char move;
    clear_terminal();
    while ( getState(mapFile) == 0) {

        clear_terminal();
        renderMap(map, rows, cols);
        
        move = getUserMove(); /*get the user input*/
        movePlayer(move, map, rows, cols, &gameInfo, mapFile); 
        
        /*for now, each time player makes a Valid move, we move both the wolf and snake as well*/
        moveSnake(map, rows, cols, &gameInfo, mapFile);
        moveWolf(map, rows, cols, &gameInfo, mapFile);
    }
    
    clear_terminal();
    renderMap(map, rows, cols);
    newSleep(0.25); /*make printing the result smoother*/

    if (gameInfo.state == 1) 
        write(1, "You win!\n", 9);
    else 
        write(1, "HAHAHA loser\n", 13);


    /*close the copy file at the very end and then return, do other clean up as well*/
    freeMap(map, rows, cols); /*DEBUG - move to the end later, clean up also needs to be done in case of ^c*/
    close(mapFile);
    return 0;
}
