#include "game.h"

/*function to accept char from the user*/
char getUserMove(void) {
    struct termios old, new;

    /*get the current terminal settings*/
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO); /*disable canonical mode and echo*/
    /*set the new terminal settings*/
    tcsetattr(STDIN_FILENO, TCSANOW, &new);

    /*get the user input*/
    char move;
    read(STDIN_FILENO, &move, 1); /*read a single char from stdin*/

    /*restore orignal terminal*/
    tcsetattr(STDIN_FILENO, TCSANOW, &old);

    return move;
}

/*takes a fd, returns the state of the game*/
int getState (int fd) {
    char state; 
    lseek(fd, 0, SEEK_SET); /*reset the file pointer to the start of the file*/
    read(fd, &state, 1); /*read the state*/

    return singleDigitCharToInt(state); /*convert the string just read into an int and return it*/
}

/*helper function for movePlayer, takes the new positions and makes apropriate changes*/
void moveP (char** map, int rows, int cols, int x, int y, GameInfo* gameInfo, int fd) {
    /* check if x and y are out of index*/
    if (x >= 0 && x < rows && y >= 0 && y < cols ) {

        /*might remove the 2 lose conditions later, idk tho*/

        if (map[x][y] == '~') /*overlappiing with snake? LOSE */ 
            gameInfo->state = 2;  /*incase player is dumb and walks right into snake*/

        if (map[x][y] == 'W') /*overlappiing with wolf? LOSE */ 
            gameInfo->state = 2;  /*incase player is dumb and walks right into wolf*/
        
        /*checks for * # */  
        if (map[x][y] != '*' && map[x][y] != '#' ) {

            /*clear old spot of p*/
            map[gameInfo->px][gameInfo->py] = ' ';

            /*update the p both in struct AND map*/
            gameInfo->px = x;
            gameInfo->py = y;

            map[x][y] = 'P';

            /*once a VALID move is made, check the following*/

            /*check for goal*/
            if (x == gameInfo->goalx && y == gameInfo->goaly) {
                gameInfo->state = 1;
            }

        } 
    }

    /*once move is made, and array and gameinfo has been updated, then update the file*/
    updateFile(fd, map, rows, cols, gameInfo);
}

/*simple decides the coordinate of the move, and also if the move should be ignored or not*/
void movePlayer(char move, char** map, int rows, int cols, GameInfo* gameInfo, int fd) {
    int x = -1;
    int y = -1;
    
    switch (move) {
        case 'w':
            /*UP*/
            x = gameInfo->px -1;
            y = gameInfo->py;

            /*call the helper function*/
            moveP (map, rows, cols, x, y, gameInfo, fd);          
            break;
        case 's':
            /*DOWN*/
            x = gameInfo->px +1;
            y = gameInfo->py;    

            /*call the helper function*/
            moveP (map, rows, cols, x, y, gameInfo, fd);
            break;
        case 'a':
            /*LEFT*/
            x = gameInfo->px;
            y = gameInfo->py-1;      

            /*call the helper function*/
            moveP (map, rows, cols, x, y, gameInfo, fd);  
            break;
        case 'd':
            /*RIGHT*/
            x = gameInfo->px;
            y = gameInfo->py+1;         

            /*call the helper function*/
            moveP (map, rows, cols, x, y, gameInfo, fd);  
            break;
        default : 
            break;
        }

}

/*takes new x and y coordinates for snake, and updtaes relevant info if valid move*/
void moveS (char** map, int rows, int cols, int x, int y, GameInfo* gameInfo, int fd) {

    /*check if player has lost*/
    if (map[x][y] == 'P') /*overlappiing with P? LOSE */ 
        gameInfo->state = 2;  
    
    /*clear old spot of S*/
    map[gameInfo->snakex][gameInfo->snakey] = ' ';

    /*update the ~ both in struct AND map*/
    gameInfo->snakex = x;
    gameInfo->snakey = y;

    map[x][y] = '~';
    
    /*once move is made, and array and gameinfo has been updated, then update the file*/
    updateFile(fd, map, rows, cols, gameInfo);
}

/*move randomly one step in any availabe 8 directions, move every 2.5 sec*/
/*helper function for moveS*/
void moveSnake (char** map, int rows, int cols, GameInfo* gameInfo, int fd) {
    int x = -1;
    int y = -1;
    
    /*get the current coordinates of snake*/
    int sX = gameInfo->snakex;
    int sY = gameInfo->snakey;

    /*array of valid moves - indexes 0 - 7 (make array of8 moves, then choose a random index )*/
    /*2d array of ints , 8 by 2 matrix of cooridiantes */
    int moves[8][2] = {{sX-1, sY-1}, {sX-1, sY}, {sX-1, sY+1}, {sX, sY+1}, {sX+1, sY+1}, {sX+1, sY}, {sX+1, sY-1}, {sX, sY-1}};

    /*now choose a randome index (0 to 7)*/
    int validMove = 0;
    int index;
    int move[2]; 
    while (!validMove) {
        index = randomUCP(0, 7);
        x = moves[index][0];
        y = moves[index][1]; 
        /* flip the flag if a valid move was selected and exit while loop*/
        /* check if x and y are out of index*/
        /*also check if we're in any invalid area */
        if (x >= 0 && x < rows && y >= 0 && y < cols) {
            /*checks for * # G W*/  
            if (map[x][y] != '*' && map[x][y] != '#' && map[x][y] != 'G' && map[x][y] != 'W' ) {
                move[0] = x;
                move[1] = y;
                validMove = 1;
            }
        }
    }

    moveS(map, rows,cols, x, y, gameInfo, fd);
}

// /*move randomly one step in any availabe 4 directions, move every 1 sec*/
// void moveWolf () {

// }

/*takes a map array, new state, and open fd*/
/*writes contents of array to fd*/
/*updtaes the file, and state*/
int updateFile (int fd, char** map, int rows, int cols, GameInfo* gameInfo) {
    if (fd < 0 || map == NULL) {
        write(2, "updateFile : invalid Fd or 2d array\n", 36 );
        return -1;
    }

    lseek(fd, 0, SEEK_SET);

    /*1 - write the current state of the game*/
    int s = gameInfo->state;
    char state = singleDigitIntToChar(s);
    write(fd, &state, 1);
    write(fd, "\n", 1);

    /*2 - write the rows and cols*/
    /*minus 2 to un account for the borders*/
    char* row = twoDigitIntToChar(rows-2);
    char* col = twoDigitIntToChar(cols-2);
    write(fd, row, 2);
    write(fd, " ", 1);
    write(fd, col, 2);
    free(row);
    free(col);

    /*3 - write the actual matrix using nested loop*/
    int r, c;
    char ch;
    for (r = 0; r < rows; r++) {
        
        /*ignore the border*/
        for (c = 0; c < cols; c++) {            
            if (r == 0 || r == rows-1 || c == 0 || c == cols - 1) {
                continue;
            }  

            switch (map[r][c]) {
                case ' ': 
                    ch = '0';
                    break; 
                case '#': 
                    ch = '1';
                    break; 
                case 'P': 
                    ch = '2';
                    break; 
                case 'G': 
                    ch = '3';
                    break; 
                case '~': 
                    ch = '4';
                    break;
                case 'W': 
                    ch = '5';
                    break;
            }
            write(fd, &ch, 1);

            /*do not print a space after the last char*/
            if (c == cols -1) 
                break;
            
            write(fd, " ", 1);
        }

        /*do not print new line after the last row*/
        if (r == rows - 1) 
            break;

        write(fd, "\n", 1);
    }

    return 0; /*success*/

}


