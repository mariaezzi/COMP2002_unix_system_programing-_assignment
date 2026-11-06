#include "fileHandling.h"

/*------------------------------------- HELPERS ---------------------------------------*/

/*returns the length of a string, not including the null terminator*/
int stringLen(char* str) {
    int len = 0;
    int i;
    for (i = 0; str[i] != '\0' ; i++) { len++; }

    return len;
}

/*converts a single digit int to a char*/
char singleDigitIntToChar(int num) {
    /*trick only works for a single digit numbers*/
    /*take the decimal value of 0 (which is 48), and add the value of the num (eg 3) to it*/
    /*which is 51*/
    /*result is ascii prowsable value of the initial number*/
    return '0' + num;
}
/*converts a single digit char to an int*/
int singleDigitCharToInt(char ch) {
    return ch - '0';
}

/*takes a 2 digit int, returns a char* representation of it*/
char* twoDigitIntToChar(int num) {
    char *result = malloc(3); /*allocate memory for 2 digits + null terminator*/
    if (result == NULL) {
        write(2, "Memory allocation failed\n", 26);
        return NULL;
    }
    
    result[0] = singleDigitIntToChar(num / 10); /*get the first digit*/
    result[1] = singleDigitIntToChar(num % 10); /*get the second digit*/
    result[2] = '\0'; /*null terminate the string*/
    return result;
}

/*takes a 2 digit char*, returns the int representation of it*/
int twoDigitCharToInt(char* str) {
    if (str == NULL || stringLen(str) != 2) {
        write(2, "Invalid input for twoDigitCharToInt\n", 36);
        return -1; /*error code for invalid input*/
    }

    int res = 0;
    res = res * 10 + (str[0] - '0'); /*convert first char to int and multiply by 10*/
    res = res * 10 + (str[1] - '0'); /*convert second char to int and add to result*/
    return res;
}

void setWhiteBackground() {
    write(1, "\033[47m", 5); /* ANSI escape code for white background */
}

void resetBackground() {
    write(1, "\033[0m", 4); /* ANSI escape code to reset background */
}

void clear_terminal(void) {
    // \033[2J clears the entire screen
    // \033[H moves the cursor back to the top-left corner (home)
    write(STDOUT_FILENO, "\033[2J\033[H", 7);
}

/*--------------------------------- MAIN OPERATIONS -----------------------------------*/

int* getRowsAndCols(int fd) {
    char rows[3] = {0,0,'\0'}; 
    char cols[3] = {0,0,'\0'};
    char ch;     

    lseek(fd, 2, SEEK_SET); /*skip state and new line*/

    read(fd, &rows, 2); /*read the rows*/
    read(fd, &ch, 1); /*skip space*/
    read(fd, &cols, 2); /*read the cols*/
    read(fd, &ch, 1); /*skip new line*/

    /*convert the strings just read into ints*/
    /*plus 2 to account for borders on both ends*/
    int r = twoDigitCharToInt(rows) + 2;
    int c = twoDigitCharToInt(cols) + 2;

    int*result = malloc(2 * sizeof(int)); /*allocate memory for 2 ints*/
    if (result == NULL) {
        write(2, "Memory allocation failed\n", 26);
        return NULL;
    }
    
    result[0] = r;
    result[1] = c;

    return result; /*return the rows and cols as an int array*/

}

/*takes fd, creates a copy, return the copied fd*/
int copyFile (char* file) {

    int fd_orignal = open(file, O_RDONLY);
    if (fd_orignal < 0) {
        write(2, "open map.txt failed\n", 20 );
        return fd_orignal;
    }
    
    /* Creates file with permissions 0644 (read/write for owner, read-only for others)*/
    int fd_copy = open("state.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd_copy < 0) {
        write(2, "creating state.txt failed\n", 26 );
        return fd_copy;
    }

    // now copy the file a single byte at a time
    char ch;
    while (read(fd_orignal, &ch, 1) > 0) {
        write (fd_copy, &ch, 1);
    }

    close(fd_orignal);

    lseek(fd_copy, 0, SEEK_SET); /*reset the file pointer to the start of the file*/

    /*if all goes well, return the open fd_copy*/
    return fd_copy;
}

/*takes a fd, reads map contents, returns 2d array representaion*/
void loadMap(char** map, int fd, int rows, int cols, GameInfo* gameInfo) {

    char ch; /*used to read in characters from the file*/

    char symbols[6] = {' ', '#', 'P', 'G', '~', 'W'};

    /*read the sate*/    
    gameInfo->state = getState(fd);

    /*skip the state, the rows and cols*/
    /* state(1) + newline + 2digits + space + 2digits + newline*/
    lseek(fd, 8, SEEK_SET); 

    int r, c;
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            /*handle the outer border*/
            if (r == 0 || r == rows-1 || c == 0 || c == cols - 1) {
                map[r][c] = '*';
            }            

            else {
                read(fd, &ch, 1); /*read the next character from the file*/
                int symbolIndex = singleDigitCharToInt(ch); /*convert char to int and use as index for symbols array*/
                map[r][c] = symbols[symbolIndex]; /*store corresponding symbol in the 2D array*/
                read(fd, &ch, 1); /*skip space or newline after character*/

                /*load data into game info struct*/
                switch (map[r][c]){
                case 'P':
                    gameInfo->px = r;
                    gameInfo->py = c;
                    break;
                case 'G' : 
                    gameInfo->goalx = r;
                    gameInfo->goaly = c;
                    break;
                case '~' : 
                    gameInfo->snakex = r;
                    gameInfo->snakey = c;                    
                    break;
                case 'W' : 
                    gameInfo->wolfx = r;
                    gameInfo->wolfy = c;                    
                    break;
                default:              
                    break;
                }
            }
        }
    }
    

}

/*takes a 2d array map, renders it out to the terminal*/
int renderMap (char** map, int rows, int cols) {
    int r, c;
    char ch;

    /*error checking*/
    if (map == NULL) {
        write(2, "renderMap: map is NULL\n", 24);
        return -1;
    }

    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            ch = map[r][c];
            
            /*set white background for #*/
            if (ch == '#') {
                setWhiteBackground();
                write(1, " ", 1);
                resetBackground(); 
            }
            /*other wise, just simply print the char*/
            else {
                write(1, &ch, 1);
            }
        }
        write(1, "\n", 1); /*new line after each row*/
    }

    write(1, "Press w to move UP\n", 19);
    write(1, "Press s to move DOWN\n", 21);
    write(1, "Press a to move LEFT\n", 21);
    write(1, "Press d to move RIGHT\n", 22);

    return 0; /*success*/
}

/*takes a 2d array map, and then frees it */
int freeMap (char** map, int rows, int cols){
    
    if (map == NULL) {
        write(2, "freeMap: map is NULL\n", 21);
        return -1;
    }

    int i;
    for (i = 0; i < rows; i ++) {
        free(map[i]);
    }
    free(map);

    return 0; /*success*/
}
