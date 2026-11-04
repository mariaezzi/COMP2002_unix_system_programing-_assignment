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
void moveP (char** map, int rows, int cols, int x, int y, GameInfo* gameInfo) {
        /* check if x and y are out of index*/
        if (x >= 0 && x < rows && y >= 0 && y < cols ) {

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
}

/*simple decides the coordinate of the move, and also if the move should be ignored or not*/
int movePlayer(char move, char** map, int rows, int cols, GameInfo* gameInfo) {
    int x = -1;
    int y = -1;
    
    switch (move) {
        case 'w':
            /*UP*/
            x = gameInfo->px -1;
            y = gameInfo->py;

            /*call the helper function*/
            moveP (map, rows, cols, x, y, gameInfo);          
            break;
        case 's':
            /*DOWN*/
            x = gameInfo->px +1;
            y = gameInfo->py;    

            /*call the helper function*/
            moveP (map, rows, cols, x, y, gameInfo);
            break;
        case 'a':
            /*LEFT*/
            x = gameInfo->px;
            y = gameInfo->py-1;      

            /*call the helper function*/
            moveP (map, rows, cols, x, y, gameInfo);  
            break;
        case 'd':
            /*RIGHT*/
            x = gameInfo->px;
            y = gameInfo->py+1;         

            /*call the helper function*/
            moveP (map, rows, cols, x, y, gameInfo);  
            break;
        default : 
            break;
        }

    
}

// /*takes a map array, new state, and open fd*/
// /*writes contents of array to fd*/
// /*updtaes the file, and state*/
// int updateFile () {
//     return 0;
// }

