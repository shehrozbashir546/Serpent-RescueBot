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

    int moveup=0;  
    int moveright=0;
    int movedown=0;
    int moveleft=0;

    checkpoint:
    //if T has bigger y position index than R in the array
    if((vR-vT) > 0 && NoVertical == false){ 
        printf("going up! \n");
        //if there is an obstacle above R
        if (world[rup] == '#' || forbidden(rup)) { 
            //check for obstacles on both sides too (if its cornered) and add it to the blacklist if yes
            if (((world[rleft]) == '#' || forbidden(rleft)) && (world[rright] == '#' || forbidden(rright))){
                blacklist(robot_index);
                if(driveMode(world[rdown])) return drivemode; //change to water if water detected
                return down; //go down
            }

            //if both right and left are free, check how long it would take to go that direction then up
            if (world[rright] != '#' || forbidden(rright) == false) {
                int i;
                //to avoid infinite loop we have the conditions:
                //if up then right is free, count straight up
                if (world[rup+1] != '#') {i = (rup);}
                //if not, start from up then right
                else {i = (rup+1);}
                //loop up till you find a free path and store that in moveright var.
                for(i; i > 1; i= i-width-1) {
                    countright++;
                    moveright++;
                    if (world[i] != '#') break;
                }
            } 

            if (world[rleft] != '#' || forbidden(rleft) == false) {
                int i;
                if (world[rup-1] != '#') {i =(rup);}
                else {i = (rup-1);}
            
                for(i; i>1; i= i-width-1) {
                    countleft++;
                    moveleft++;
                    if (world[i] != '#') break;
                }
            } 
            
            //if it takes less steps to go left, or its the same as right, go left then up
            if (countleft < countright || countleft == countright) {
                //go left until up is clear
                do{ 
                    if (driveMode(world[rleft])) return drivemode; 
                    return left;
                }
                while(world[rleft] != '#');
                //move up according to count
                for (int i=0; i<moveleft;i++){
                    return up;
                }
            }
            
            //if it takes less steps to go right then up, do it
            if (countleft > countright ) {
                //go right until up is clear
                do{ 
                    if (driveMode(world[rright])) return drivemode;
                    return right;
                }
                while(world[rright] != '#');
                    
                for (int i=0; i<moveright;i++){
                    if(driveMode(world[rup])) return drivemode;
                    return up;
                }
                }  
        }
        
        //when no obstacles:
        noHorizontal=0;
        if(driveMode(world[rup])) return drivemode;
        return up;
    }   

    //DOWN
    if ((vR-vT) < 0 && NoVertical == false){  
        printf("going down!\n");
        //if there is an obstacle below R
        if (world[rdown] == '#' || forbidden(rdown)) {
            //check if it is cornered from the sides, if yes add to blacklist.
            if (((world[rright]) == '#' || forbidden(rright)) && (world[rleft] == '#' || forbidden(rleft))){
                blacklist(robot_index);
                if(driveMode(world[rup])) return drivemode; //check for water
                return up; //go up
            }

            //if obstacle down and right is clear
            if (world[rright] != '#' || forbidden(rright) == false) {
                //check for opening down then right
                int i;
                if (world[rdown+ 1] != '#') i = (rdown);
                else i = (rdown+1);
                for(i; i > 200; i=(i+width+1)) {
                    countright++;
                    moveright++;
                    if (world[i] != '#') break;
                }
            }

            if (world[rleft] != '#' || forbidden(rleft) == false) {
                int i;
                if (world[rdown- 1]  != '#') i = (rdown);
                else i = (rdown-1);                
                for(i; i > 200; i=(i+width+1)) {
                    countleft++;
                    moveleft++;
                    if (world[i] != '#') break;
                }
            }
            

            if (countleft < countright || countleft == countright) {
                do{
                    if (driveMode(world[rleft])) return drivemode;
                    return left;
                }
                while((world[rleft] != '#') && forbidden(rleft) == false);
                    
                for (int i=0; i<moveleft;i++){
                    if (driveMode(world[rdown])) return drivemode;
                    return down;
                }
            }

            //if it takes less steps to go right then down, do it
            if (countleft > countright ) {
                    do{
                        if (driveMode(world[rright])) return drivemode;
                        return right;                    
                    }
                    while((world[rup] == '#' || forbidden(rup)) && (world[rright]!= '#'));
                        
                    for (int i=0; i<moveright;i++){
                        if (driveMode(world[rdown])) return drivemode;
                        return down;
                    }
            }
        }
        //when no obstacles:
        noHorizontal=0;
        if(driveMode(world[rdown])) return drivemode;
        return down;   
    }
    
    else {NoVertical = 1;} //temporary lock for Vmovement

    //RIGHT
    if((hR-hT) < 0 && noHorizontal == false) {
        //if right is blocked
        printf("going right!\n");
        if(world[rright]=='#' || forbidden(rright)){
            //if blocked from up and down too, add to blacklist and go left
            if ((world[rup] == '#' || forbidden(rup)) && (world[rdown]=='#' || forbidden(rdown))){ 
                blacklist(robot_index);
                if (driveMode(world[rleft])) return drivemode;
                return left;
            }


        
            if(world[rdown] != '#' || forbidden(rdown) != true) {
                int i;
                if ((world[rdown+1]) != '#') {i = (rright);}
                else if ((world[rdown+1]) == '#'){i = (rdown+1);};
                for(i; i < 200; i++) {
                    countdown++;
                    movedown++;
                    if (world[i] != '#') break;
                }
                
            }

            if(world[rup] != '#' || forbidden(rup) != true) {
                int i;
                if (world[rup+1] != '#') i = (rright);
                else i = (rup+1);                
                for(i; i  < 200; i++) {
                    countup++;
                    moveup++;
                    if (world[i] != '#') break;
                }
            }

            
            //whichever takes less to do, execute

            if (countdown < countup || countdown == countup){
                //keep going down until right is free
                do{   
                    if(driveMode(world[rdown])) return drivemode;                  
                    return down;
                }
                while(world[rdown]!= '#' && world[rright] == '#');  
                //move the amount of steps needed to the opening
                for (int k=0; k<movedown;k++){
                    if(driveMode(world[rright])) return drivemode;
                    return right;
                }
            }

            if (countdown > countup){
                //keep going up until right is free
                do{    
                    if(driveMode(world[rup])) return drivemode;
                    return up;
                }
                while(world[rup] != '#');
                
                //move the amount of steps needed to the opening 
                for (int i=0; i<moveup;i++){
                    if(driveMode(world[rright])) return drivemode;
                    return right;
                }
            }
        }

        NoVertical=0;
        if (driveMode(world[rright])) return drivemode;
        return right;
    }


    //LEFT
    if ((hR-hT) > 0 && noHorizontal == 0)  {
        printf("going left!\n");
        //if left blocked 
        if(world[rleft]=='#' || forbidden(rleft)){
            //check if its blocked from up and down, add to blacklist if yes.
            if ((world[rup] == '#' || forbidden(rup)) && (world[rdown]=='#' || forbidden(rdown))){ 
                blacklist(robot_index);
                if (driveMode(world[rleft])) return drivemode;
                return right;
            }

 
            if(world[rdown] != '#' || forbidden(rdown) != true) {
                countdown=0;
                int i;
                if (world[rdown] - 1 != '#') i = (rdown);
                else i = (rdown-1);                   
                //loop down left until an O is found
                for(i; i > 0; i--) {
                    countdown++;
                    if (world[i] != '#') break;
                }
               // printf("it took me %d steps down left to find an O!", countdown);
            }
            
            //if left is blocked: look for an opening after going up then left, and count how many steps
            if(world[rup] != '#' || forbidden(rup) != true) {
                countup=0;
                int i;
                if (world[rup] - 1 != '#') i = (rup);
                else i = (rup-1);                   
                //loop up then left until an O is found
                for(i; i > 0; i--) {                        
                    countup++;
                    if (world[i] != '#') break;
                }
                //printf("it took me %d steps up left to find an O!", countup);
            }


            if (countdown < countup || countdown == countup){
                //keep going down until left is free
                do{                  
                    if(driveMode(world[rdown])) return drivemode;      
                    return down;
                }
                while(world[rdown] != '#');

                //move the amount of steps needed to the opening
                for (int i=0; i<movedown;i++){
                    if(driveMode(world[rleft])) return drivemode;
                    return left;
                }
            }

            if (countdown > countup){
                //keep going up until left is free
                do{      
                    if(driveMode(world[rup])) return drivemode;                  
                    return up;
                }
                while(world[rup] != '#');

                //move the amount of steps needed to the opening 
                for (int i=0; i<moveup;i++){
                    if(driveMode(world[rleft])) return drivemode;
                    return left;
                }
            }
                
        }
            // when no obstacles:
            NoVertical=0;
            if (driveMode(world[rleft])) return drivemode;
            return left;
    }

    else{noHorizontal = true;} //temporary horizontal lock

    //if deadlock occur
    if (NoVertical == true && noHorizontal == true )
    {
        printf("\nSTUCK! Resetting!");
        NoVertical = 0;
        noHorizontal = 0;
        goto checkpoint;
    }
}