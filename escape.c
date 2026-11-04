#include "stdio.h" /*DEBUG - remove later*/
#include "fileHandling.h"

int main (int argc, char* argv[]) {

    if (argc != 2) {
        write(2, "Usage: ./escape <map.txt>\n", 26);
        return 1;
    }

    /*copy and validate copied fd*/


    int mapFile = copyFile(argv[1]); 
    if (mapFile < 0) {
        write(2, "copying map.txt failed\n", 23);
        return -1;
    }

    
    /* read in rows in main to make parameter passing easier*/
    int rows, cols;
    char rows[3] = {0,0,'\0'}; 
    char cols[3] = {0,0,'\0'};
    char ch;     
    int readResult;

    lseek(fd, 2, SEEK_SET); /*skip state and new line*/

    read(fd, &rows, 2); /*read the rows*/
    read(fd, &ch, 1); /*skip space*/
    read(fd, &cols, 2); /*read the cols*/
    read(fd, &ch, 1); /*skip new line*/

    /*convert the strings just read into ints*/
    /*plus one to account for borders*/
    int rInt = twoDigitCharToInt(rows) + 2;
    int cInt = twoDigitCharToInt(cols) + 2;

    char** map = loadMap(mapFile, rInt, cInt);




}
