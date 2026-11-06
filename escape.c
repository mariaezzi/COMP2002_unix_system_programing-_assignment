#include "fileHandling.h"
#include "game.h"
#include "gameInfo.h"
#include "newSleep.h"
#include"terminal.h"


/*multi proccess includes*/
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/file.h>

/*initialize data needed for each process*/
char** initializeProcess(int fd, int rows, int cols, GameInfo* gameInfo);


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
    close(mapFile); /*job done, copy created, close the fd, since every proccess gets its own fd regardless*/


    /*disable buffer - doing only once for cleaner rendering on terminal*/
    disableBuffer();


    /* ============================ MULTI PROCCESS IMPLIMENTATION ============================ */

    int playerFd; /*for parent*/
    int pid = fork(); /*parent and first child*/
    
    /*parent - my poor player - give own copy (array and struct) and start moving*/
    if (pid > 0) {

        int pid_fork2 = fork();

        /* ========================= WOLF CHILD ========================= */
        /*child 3 - my witty wolf - give own copy (array and struct) and start moving every 1 sec*/
        if (pid_fork2 == 0) {
            /*allocate mem, load file data and struct*/
            int wolfFd = open("state.txt", O_RDWR);
            GameInfo gameInfo;
            char** map = initializeProcess(wolfFd, rows, cols, &gameInfo);
            
            /*doing this out side of loop only for parent proccess to avoid race conditions*/
            clear_terminal();
            renderMap(map, rows, cols);
            
            while(1) {
                newSleep(0.5); /*wait 1 secs every move*/
                flock(wolfFd, LOCK_EX); /*lock for other processes*/
                
                loadMap(map, wolfFd, rows, cols, &gameInfo);
                if (getState(wolfFd) != 0) {
                    flock(wolfFd, LOCK_UN); /* release for other processes */
                    break; /*exit the while loop*/
                }
                else {
                    /*make move and update*/
                    clear_terminal();
                    moveWolf(map, rows, cols, &gameInfo, wolfFd);
                    renderMap(map, rows, cols);
                    updateFile(wolfFd, map, rows, cols, &gameInfo);
                    
                    flock(wolfFd, LOCK_UN); /* release for other processes */

                    /*if game is lost, dont go back into while loop again and sleep*/
                    if (gameInfo.state != 0)
                        break;
                }
            }
            
            freeMap(map, rows, cols);
            close(wolfFd);
            exit(0);
        }


        /* ========================= PARENT PLAYER ========================= */

        /*allocate mem, load file data and struct - for player */
        playerFd = open("state.txt", O_RDWR);
        GameInfo gameInfo;
        char** map = initializeProcess(playerFd, rows, cols, &gameInfo);
        char move;

        while(1) {
            move = getUserMove(); /*get the user input - initial function changed to deal with lose condiion lag*/

            flock(playerFd, LOCK_EX); /*lock for other processes*/
            
            loadMap(map, playerFd, rows, cols, &gameInfo);
            if (getState(playerFd) != 0) {
                flock(playerFd, LOCK_UN); /* release for other processes */
                break; /*exit the while loop*/
            }
            else {
                /*make move and update*/
                int moved = movePlayer(move, map, rows, cols, &gameInfo, playerFd);
                if (moved) {
                    clear_terminal();
                    renderMap(map, rows, cols);
                    updateFile(playerFd, map, rows, cols, &gameInfo);
                }
                flock(playerFd, LOCK_UN); /* release for other processes */
            }
            /*avoid waiting for new key again if game is over, and dont go into the loop again eitehr*/
            if (gameInfo.state != 0)
                break;
        }

        freeMap(map, rows, cols);
    }

    /* ========================= SNAKE CHILD ========================= */
    /*child 2 - my sneaky snake, give it its own copy (array and struct) and start moving it, every 2 secs*/
    else if (pid == 0) { 
        /*allocate mem, load file data and struct*/
        int snakeFd = open("state.txt", O_RDWR);
        GameInfo gameInfo;
        char** map = initializeProcess(snakeFd, rows, cols, &gameInfo);
        

        while(1) {
            newSleep(1); /*wait 2 secs every move*/
            flock(snakeFd, LOCK_EX); /*lock for other processes*/
            
            loadMap(map, snakeFd, rows, cols, &gameInfo);
            if (getState(snakeFd) != 0) {
                flock(snakeFd, LOCK_UN); /* release for other processes */
                break; /*exit the while loop*/
            }
            else {
                /*make move and update*/
                clear_terminal();
                moveSnake(map, rows, cols, &gameInfo, snakeFd);
                renderMap(map, rows, cols);
                updateFile(snakeFd, map, rows, cols, &gameInfo);
                
                flock(snakeFd, LOCK_UN); /* release for other processes */

                /*if game is lost, dont go back into while loop again and sleep*/
                if (gameInfo.state != 0)
                    break;
            }
        }

        freeMap(map, rows, cols);
        close(snakeFd);
        exit(0);
    }



    /* =================================== CLEAN UP ======================================== */

    /*wait for both child proceses to finish before checking condition*/
    wait(NULL);
    wait(NULL);

    if (getState(playerFd) == 1) 
        write(1, "You win!\n", 9);
    else 
        write(1, "HAHAHA loser\n", 13);

    /*close the copy file at the very end and then */
    close(playerFd);

    /*enable buffer bofore returning - restore orignal terminal*/
    enableBuffer(); 

    return 0;
}


char** initializeProcess(int fd, int rows, int cols, GameInfo* gameInfo) {
    /*initialize 2d array*/
    char** map = (char**)malloc(rows * sizeof(char*));
    if (map == NULL) {
        write(2, "Map Memory allocation failed\n", 29);
    }
    int i;
    for (i = 0; i < rows; i ++) {
        map[i] = (char*)malloc(cols * sizeof(char));
    }
    
    /*load gama data into array and struct*/
    loadMap(map , fd, rows, cols, gameInfo); 
    return map;
}