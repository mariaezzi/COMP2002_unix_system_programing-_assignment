#include "fileHandling.h"
#include "game.h"

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


    int* res = getRowsAndCols(mapFile);
    int rows = res[0];
    int cols = res[1];
    free(res); /*free the memory allocated for res*/

    char** map = loadMap(mapFile, rows, cols); /*also handles closing of mapFile*/
    int renderResult = renderMap(map, rows, cols);
    int freeResult = freeMap(map, rows, cols);
    
    char move;
    while (1) {
        move = getUserMove(); /*get the user input*/
        write(1, &move, 1); /*write the user input to stdout*/
        write(1, "\n", 1); /*write a new line to stdout*/
    }

}
