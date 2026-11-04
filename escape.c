#include "stdio.h" /*DEBUG - remove later*/
#include "fileHandling.h"

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

    
    /* read in rows and cols in main to make parameter passing easier*/
    char rows[3] = {0,0,'\0'}; 
    char cols[3] = {0,0,'\0'};
    char ch;     
    int readResult;

    lseek(mapFile, 2, SEEK_SET); /*skip state and new line*/

    read(mapFile, &rows, 2); /*read the rows*/
    read(mapFile, &ch, 1); /*skip space*/
    read(mapFile, &cols, 2); /*read the cols*/
    read(mapFile, &ch, 1); /*skip new line*/

    /*convert the strings just read into ints*/
    /*plus one to account for borders*/
    int r = twoDigitCharToInt(rows) + 2;
    int c = twoDigitCharToInt(cols) + 2;

    char** map = loadMap(mapFile, r, c); /*also handles closing of mapFile*/
    int renderResult = renderMap(map, r, c);



}
