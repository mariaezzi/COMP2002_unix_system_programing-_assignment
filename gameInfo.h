#ifndef GAMEINFO_H
#define GAMEINFO_H

typedef struct {
    int px; /*track x coordinate of player*/
    int py; /*track y coordinate of player*/   
    int snakex; /*track x coordinate of snake*/
    int snakey; /*track y coordinate of snake*/
    int wolfx; /*track y coordinate of wolf*/
    int wolfy; /*track y coordinate of wolf*/
    int goalx; /*track x coordinate of goal*/
    int goaly; /*track y coordinate of goal*/
    int state; /*track state of the game*/ 
    /*  0 - ongoing 
        1 - win
        2 - lost 
    */
} GameInfo;

#endif