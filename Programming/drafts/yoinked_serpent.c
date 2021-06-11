#include "robot_serpent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// ALLOWED RETURN VALUES:
// 1: North, 2: East, 3: South, 4: West, 5: Toggle watern/land mode
void blacklist(int blacklisted){
    forbiddenlist[counter] = blacklisted; //mark location that surrounded by obstacle
    counter++;
}

int forbidden(int forbiddenLocation){
    //for loop checks if location is in forbiddenlist array.
    for (int i = 0; forbiddenlist[i] != '\0'; i++){
        if (forbiddenLocation == forbiddenlist[i]) return true; //location blacklisted
    }
    return false;
}

int  driveMode(char infront){
    //if robot is on land, and in front of it is water, driveMode == true
    if (infront == '~' && land){
        land = 0;
        water = 1;
        return true;
    }

    if (infront == 'O' && water){
        land = 1;
        water = 0;
        return true;
    }

    return false;
}

int move(char *world) {
    // copy the array into a new one to get the size
    char worldcpy[200];
    for(int i=0;i<200;i++) {
        worldcpy[i] =world[i];
    }
    
    unsigned int elements = sizeof(worldcpy)/sizeof(worldcpy[0]);
    int robot_index;
    int target_index;
    int lines=0;
    int width=20;

    for(int i = 0; i < elements; ++i) { //index of R
        if (world[i] == 'R') {
            robot_index = i;
            break;
        }
    }

    for(int i = 0; i < elements; ++i) { //index of T
        if (world[i] == 'T') {
            target_index = i;
            break;
        }
    }

    int rup = robot_index - width - 1;
    int rdown = robot_index + width + 1;
    int rleft = robot_index - 1;
    int rright = robot_index + 1;
    int rsurround[4] = {rup,rright,rdown,rleft};

    int hT; //horizontal T
    int vT; //vertical T
    int hR; //horizontal R;
    int vR; //vertical R
    //calculate horizontal and vertical T position
    hT = target_index % 21;
    vT = (target_index / 21) + 2;

    //calculate horizontal and vertical R position
    hR = robot_index % 21;
    vR = (robot_index / 21) + 2;

    int mdown=0;
    int mup=0;
    int mleft=0;
    int mright=0;

    checkpoint:
    //if T has bigger vertical position index than R in the array
    if((vR-vT) > 0 && NoVertical == false){ 
        printf("going up! \n");
        //if there is an obstacle above R
        if (world[rup] == '#' || forbidden(rup)) { 
            //check for obstacles on both sides too (if its cornered) and add it to the blacklist if yes
            if (((world[rleft]) == '#' || forbidden(rleft)) && (world[rright] == '#' || forbidden(rright))){
                blacklist(robot_index);
                mleft=0;
                mright=0;
                if(driveMode(world[rup])) return drivemode; //change to water if water detected
                return down; //go down
            }
        

            //if right is blocked, but left is free, move left then add to blacklist.
            else  if ((world[rright]== '#' || forbidden(rright)) && (world[rleft] != '#')){
                mright=0;
                mleft=1;
                blacklist(robot_index);
            }

            //if left is blocked but right is free, move right then add to blacklist
            else if ((world[rleft]== '#' || forbidden(rleft)) && (world[rright] != '#')){
                mright=1;
                mleft=0;
                blacklist(robot_index);
            }

            //if both right and left are free, mark them both available
            else if (world[rright] != '#' || forbidden(rright) == false) mright = 1;
            else if (world[rleft] != '#' || forbidden(rleft) == false) mleft = 1;
                
            //pick if left or right
            if (mright == 1){
                if (driveMode(world[rright])) return drivemode;
                return right;
            }

            else if (mleft == 1){
                //check for method change
                if (driveMode(world[rleft])) return drivemode;
                return left;
            }    
        }
        
        //when no obstacles:
        mleft=0;
        mright=0;
        noHorizontal=0;
        if(driveMode(world[rup])) return drivemode;
        return up;
    }   

    //DOWN
    if ((vR-vT) < 0 && NoVertical == false){  
        printf("DOWN");
        //if there is an obstacle below R
        if (world[rdown] == '#' || forbidden(rdown)) {
            //check if it is cornered from the sides, if yes add to blacklist.
            if (((world[rright]) == '#' || forbidden(rright)) && (world[rleft] == '#' || forbidden(rleft))){
                blacklist(robot_index);
                mleft=0; //reset
                mright=0;
                if(driveMode(world[rup])) return drivemode; //check for water
                return up; //go up
            }
        }
        //if right is free and left clear mark left available and right not
        else  if ((world[rright]== '#' || forbidden(rright)) && (world[rleft] != '#')){
            mright=0;
            mleft=1;
            blacklist(robot_index);
        }

        //if left blocked but right clear mark right available
        else if ((world[rleft]== '#' || forbidden(rleft)) && (world[rright] != '#')){
            mright=1;
            mleft=0;
            blacklist(robot_index);
        }

        //if both free make "available"
        else if (world[rright] != '#' || forbidden(rright) == false) mright = 1;
        else if (world[rleft] != '#' || forbidden(rleft) == false) mleft = 1;
            
        //action
        if (mright == 1){
            if (driveMode(world[rright])) return drivemode;
            return right;
        }
            
        else if (mleft == 1){
        if (driveMode(world[rleft]))return drivemode;
        return left;
        }     
            
        //when no obstacles:
        mleft=0;
        mright=0;
        noHorizontal=0;
        if(driveMode(world[rup])) return drivemode;
        return down;   
    }
        else {NoVertical = 1;} //temporary lock for Vmovement

    //RIGHT
    if((hR-hT) < 0 && noHorizontal == false) {
        //if right is blocked
        printf("GOING RIGHT");
        if(world[rright]=='#' || forbidden(rright)){
            //if blocked from up and down too, add to blacklist and go left
            if ((world[rup] == '#' || forbidden(rup)) && (world[rdown]=='#' || forbidden(rdown))){ 
                blacklist(robot_index);
                mup = 0 ;
                mdown = 0;
                if (driveMode(world[rleft])) return drivemode;
                return left;
            }

            //if up blocked but down is free, mark down available, up not
            else if ((world[rup] == '#' || forbidden(rup)) && (world[rdown] != '#')){
                mdown=1;
                mup=0;
                blacklist(robot_index);
            }
            //if down blocked and up free, up available, down not, don't come back there.
            else if ((world[rdown] == '#' || forbidden(rdown)) && (world[rup] != '#')){
                mup=1;
                mdown=0;
                blacklist(robot_index);
            }

            //if both available mark them both available
            else if(world[rdown] != '#' || forbidden(rdown) != true) mup = 1;
            else if(world[rup] != '#' || forbidden(rup) != true) mdown = 1;

            if (mdown == true){
                if (driveMode(world[rdown])) return drivemode;
                return down;
            }
            
            else if (mup == true) {
                 if (driveMode(world[rup])) return drivemode;
                 return up;
            }
        }

        // when no obstacles:
        mup=0;
        mdown=0;
        NoVertical=0;
        if (driveMode(world[rright])) return drivemode;
        return right;
    }


    //LEFT
    else if ((hR-hT) > 0 && noHorizontal ==0)  {
        printf("MOVING LEFT");
        //if left blocked 
        if(world[rleft]=='#' || forbidden(rleft)){
            //check if its blocked from up and down, add to blacklist if yes.
            if ((world[rup] == '#' || forbidden(rup)) && (world[rdown]=='#' || forbidden(rdown))){ 
                blacklist(robot_index);
                mup = 0 ;
                mdown = 0;
                if (driveMode(world[rleft])) return drivemode;
                return right;
            }
            //mark down available if only down is available
            else if ((world[rup] == '#' || forbidden(rup)) && (world[rdown] == 'O' || world[rdown]=='~')){
                mdown=1;
                mup=0;
                blacklist(robot_index);
            }

            //mark up available if only up is available
            else if ((world[rdown] == '#' || forbidden(rdown)) && (world[rup] == 'O' || world[rup]=='~')){
                mup=1;
                mdown=0;
                blacklist(robot_index);
            }

            //if both available mark them both
            else if(world[rdown] != '#' || forbidden(rdown) != true) mdown = 1;
            else if(world[rup] != '#' || forbidden(rup) != true) mup = 1;

            if (mdown == 1){
                if (driveMode(world[rdown])) return drivemode;
                return down;
            }

            else if (mup==1) {
                if (driveMode(world[rup])) return drivemode;
                return up;
            }
                
            }
            // when no obstacles:
            mup=0;
            mdown=0;
            NoVertical=0;
            if (driveMode(world[rright])) return drivemode;
            return left;
    }

    else{noHorizontal = true;} //temporary horizontal lock

    //if deadlock occur
    if (NoVertical == true && noHorizontal == true )
    {
        printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        NoVertical = 0;
        noHorizontal = 0;
        mleft = 0;
        mright = 0;
        mup = 0;
        mdown = 0;
        goto checkpoint;
    }
}