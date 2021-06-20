#include "robot_serpent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// ALLOWED RETURN VALUES:
// 1: North, 2: East, 3: South, 4: West, 5: Toggle watern/land mode

void blacklist(int blacklisted){
    forbiddenlist[badcounter] = blacklisted; //mark location that surrounded by obstacle
    badcounter++;
}

int forbidden(int forbiddenLocation){
    //for loop checks if location is in forbiddenlist array.
    for (int i = 0; i<200; i++){if (forbiddenLocation == forbiddenlist[i]) return true;  }
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

int destructable(char infront){
    if (infront == '*') {return true;}
    return false;
}

int current_id = -1;

int move(char *world, int map_id) {
    
    if(map_id != current_id) { 
        //reset arrays:
        current_id = map_id;
        for (int j=0;j<200;j++){forbiddenlist[j]=0; goodlist[j]=0;}
        NoVertical= 0; NoHorizontal = 0;
        badcounter = 0; goodcounter = 0;
        blacklist_reset=0; 
        countdown=0; countup=0; countleft=0; countright=0;
    }
    
    // copy the array into a new one to get the size
    char worldcpy[200];
    for(int i=0;i<200;i++) {
        worldcpy[i] =world[i];
    }
    
    unsigned int elements = sizeof(worldcpy)/sizeof(worldcpy[0]);
    int robot_index=0;
    int target_index=0;
    int xtarget=0;
    int lines=0;
    int width=20;


    for(int i = 0; i < elements; ++i) { //index of R
        if (world[i] == 'R') {
            robot_index = i;
            break;
        }
    }

  
        for(int i = 0; i < elements; ++i) { //index of X
        if (world[i] == 'X') {
            xtarget = i;
            break;
        }
        }

    for(int i = 0; i < elements; ++i) { //index of T
        if (world[i] == 'T' || world[i]=='t') {
            target_index = i;
            break;
        }
    }

    //when R reaches T, make the new target X
    if (target_index==0) {        
        target_index = xtarget; 
        blacklist_reset++;        

    }    
        
    //reset forbidden list    
    if (blacklist_reset == 1 ) { 
        printf("\nResetting Blacklist!\n");           
        for (int j=0;j<elements;j++) forbiddenlist[j]=0;
    }

    int rup = robot_index - width - 1;
    int rdown = robot_index + width + 1;
    int rleft = robot_index - 1;
    int rright = robot_index + 1;
    int rsurround[4] = {rup,rright,rdown,rleft};

    //if robot has 3 obstacles around it, add location to forbidden list.
    int obstacle=0;
    int free=0;
    int freeDirection;
    int freeLocation;

    for (int j=0; j<4;j++){
        if (world[rsurround[j]] == '#' ||  forbidden(rsurround[j])) {
            //printf("\n robot index: %d, Obstacle in this location: %d\n", robot_index, rsurround[j]);
            obstacle++;
            }

        if (world[rsurround[j]] != '#' && forbidden(rsurround[j]) == false){
            free++;
            freeDirection = j+1;
            freeLocation = rsurround[j];
        }
    } 

    if (obstacle==3) { 
        //printf("\n free direction: %d  free Location: %d obstacle: %d", freeDirection, freeLocation, obstacle);
        blacklist(robot_index);
        if(driveMode(freeLocation)) return drivemode; //change to water if water detected
        if(destructable(freeLocation)) return (freeDirection+5);
        return freeDirection; //go in the free direction
    }


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
    //UP
    if((vR-vT) > 0 && NoVertical == false){ 
        printf("going up! \n");
        //if there is an obstacle above R
        if (world[rup] == '#' || forbidden(rup)) { 
            //if both right and left are free, check how long it would take to go that direction then up
            if ((world[rright] != '#' ) || forbidden(rright) == false) {
                int i;
                //to avoid infinite loop we have the conditions:
                //if up then right is free, count straight up
                if (world[rup+1] != '#' && forbidden(rup) == false) i = (rup);
                //if not, start from up then right
                else i = (rup+1);
                //loop up till you find a free path and store that in moveright var.
                for(i; i > 1; i= i-width-1) {
                    countright++;
                    moveright++;
                    if (world[i] != '#' && forbidden(i) == false) break;
                }
            } 

            if (world[rleft] != '#' || forbidden(rleft) == false) {
                int i;
                if (world[rup-1] != '#' && forbidden((rup-1)) == false) i =(rup);
                else i = (rup-1);
            
                for(i; i>1; i= i-width-1) {
                    countleft++;
                    moveleft++;
                    if (world[i] != '#' && forbidden(i) == false) break;
                }
            } 
            
            //if it takes less steps to go left, or its the same as right, go left then up
            if (countleft < countright || countleft == countright) {
                if (((world[rleft]) == '#' || forbidden(rleft))){
                    blacklist(robot_index); 
                    if (driveMode(world[rright])) return drivemode;
                    if (destructable(world[rright])) return right_destroy;
                    if (destructable(world[rright])) {
                        if (world[rright+1] != '#' || forbidden(rright+1) == false) return right_destroy;
                        else {blacklist(rright); goto checkpoint;}
                    }
                    return right;  
                }  
                //go left until up is clear
                do{                 
                    if (driveMode(world[rleft])) return drivemode; 
                    //check if obstacle must be destroyed, destroy if yes
                    if (destructable(world[rleft])) {
                        if (world[rleft-1] != '#' || forbidden(rleft-1) == false) return left_destroy;
                        else {blacklist(rleft); goto checkpoint;}
                    }
                    return left;
                }
                while(world[rleft] != '#' && forbidden(rleft) == false);                    

                //move up according to count
                for (int i=0; i<moveleft;i++){
                    if (driveMode(world[rup])) return drivemode; 
                    if (destructable(world[rup])) {
                        if (world[rup-21] != '#' || forbidden(rup-21) == false) return up_destroy;
                        else {blacklist(rup); goto checkpoint;}
                    }
                    return up;
                }
            }
            
            //if it takes less steps to go right then up, do it
            if (countleft > countright ) {
                if (((world[rright]) == '#' || forbidden(rright))){
                    blacklist(robot_index); 
                    if (driveMode(world[rleft])) return drivemode;
                    //check if obstacle must be destroyed, destroy if yes
                    if (destructable(world[rleft])) {
                        if (world[rleft-1] != '#' || forbidden(rleft-1) == false) return left_destroy;
                        else {blacklist(rleft); goto checkpoint;}
                    }
                    return left;  
                }  
                //go right until up is clear
                do{ 
                    if (driveMode(world[rright])) return drivemode;
                    if (destructable(world[rright])) {
                        if (world[rright+1] != '#' || forbidden(rright+1) == false) return right_destroy;
                        else {blacklist(rright); goto checkpoint;}
                    }
                    return right;
                    //if robot gets cornered, go other direction
                }
                while(world[rright] != '#'&& forbidden(rright) == false);

                for (int i=0; i<moveright;i++){
                    if(driveMode(world[rup])) return drivemode;
                    if (destructable(world[rup])) {
                        if (world[rup-21] != '#' || forbidden(rup-21) == false) return up_destroy;
                        else {blacklist(rup); goto checkpoint;}
                    }
                    return up;
                }
                }  
        }
        
        //when no obstacles:
       
        if(driveMode(world[rup])) return drivemode;
        if (destructable(world[rup])) {
            if (world[rup-21] != '#' || forbidden(rup-21) == false) return up_destroy;
            else {blacklist(rup); goto checkpoint;}
        }
        return up; 
        NoHorizontal=0;
    }   

    //DOWN
    if ((vR-vT) < 0 && NoVertical == false){  
        printf("going down!\n");
        //if there is an obstacle below R
        if (world[rdown] == '#' || forbidden(rdown)) {
            //check if it is cornered from the sides, if yes add to blacklist.

            //if obstacle down and right is clear
            if ((world[rright] != '#') || forbidden(rright) == false) {
                //check for opening down then right
                int i;
                if (world[rdown+ 1] != '#' && forbidden((rdown+1)) == false) i = (rdown);
                else i = (rdown+1);
                for(i; i > 200; i=(i+width+1)) { 
                    countright++;
                    moveright++;
                    if (world[i] != '#' && forbidden(i) == false) break;
                }
            }

            if ((world[rleft] != '#') || forbidden(rleft) == false) {
                int i;
                if (world[rdown- 1]  != '#' && forbidden((rdown-1)) == false) i = (rdown);
                else i = (rdown-1);                
                for(i; i > 200; i=(i+width+1)) {
                    countleft++;
                    moveleft++;
                    if (world[i] != '#'  && forbidden(i) == false) break;
                }
            }
            

            if (countleft < countright || countleft == countright) {    
                if (((world[rleft]) == '#' || forbidden(rleft))){
                    blacklist(robot_index); 
                    if (driveMode(world[rright])) return drivemode;
                    if (destructable(world[rright])) {
                        if (world[rright+1] != '#' || forbidden(rright+1) == false) return right_destroy;
                        else {blacklist(rright); goto checkpoint;}
                    }
                    return right;  
                }  
                do{                     
                    if (driveMode(world[rleft])) return drivemode;
                    //check if obstacle must be destroyed, destroy if yes
                    if (destructable(world[rleft])) {
                        if (world[rleft-1] != '#' || forbidden(rleft-1) == false) return left_destroy;
                        else {blacklist(rleft); goto checkpoint;}
                    }
                    return left;
                }
                while((world[rleft] != '#') && forbidden(rleft) == false);
              

                for (int i=0; i<moveleft;i++){
                    if (driveMode(world[rdown])) return drivemode;
                    if (destructable(world[rdown])) {
                        if (world[rdown+21] != '#' || forbidden(rdown+21) == false) return down_destroy;
                        else {blacklist(rdown); goto checkpoint;}
                    }
                    return down;
                }
            }

            //if it takes less steps to go right then down, do it
            if (countleft > countright ) {
                if (((world[rright]) == '#' || forbidden(rright))){
                    blacklist(robot_index);
                    //check if obstacle must be destroyed, destroy if yes
                    if (destructable(world[rleft])) {
                        if (world[rleft-1] != '#' || forbidden(rleft-1) == false) return left_destroy;
                        else {blacklist(rleft); goto checkpoint;}
                    }
                    return left;
                }    
                do{
                    if (driveMode(world[rright])) return drivemode;
                    if (destructable(world[rright])) {
                        if (world[rright+1] != '#' || forbidden(rright+1) == false) return right_destroy;
                        else {blacklist(rright); goto checkpoint;}
                    }
                    return right;                    
                }
                while(world[rright]!= '#' && forbidden(rright) == false);

                for (int i=0; i<moveright;i++){
                    if (driveMode(world[rdown])) return drivemode;
                    if (destructable(world[rdown])) {
                        if (world[rdown+21] != '#' || forbidden(rdown+21) == false) return down_destroy;
                        else {blacklist(rdown); goto checkpoint;}
                    }
                    return down;
                }
            }
        }
        //when no obstacles:
        
        if(driveMode(world[rdown])) return drivemode;
        if (destructable(world[rdown])) {
            if (world[rdown+21] != '#' || forbidden(rdown+21) == false) return down_destroy; 
            else {blacklist(rdown); goto checkpoint;}
        }
        printf("default");
        return down;   
        NoHorizontal=0;
    }
    
    else {NoVertical = 1;} //temporary lock for Vmovement

    //RIGHT
    if((hR-hT) < 0 && NoHorizontal == false) {
        //if right is blocked
        printf("going right!\n");
        if(world[rright]=='#' || forbidden(rright)){
            //if blocked from up and down too, add to blacklist and go left

            if((world[rdown] != '#') || forbidden(rdown) != true) {
                int i;
                if ((world[rdown+1]) != '#' && forbidden((rdown+1)) == false) {i = (rright);}
                else i = (rdown+1);
                for(i; i < 200; i++) {
                    countdown++;
                    movedown++;
                    if (world[i] != '#' && forbidden(i) == false) break;
                }
                
            }

            if((world[rup] != '#') || forbidden(rup) != true) {
                int i;
                if (world[rup+1] != '#'  && forbidden((rup+1)) == false) i = (rright);
                else i = (rup+1);                
                for(i; i  < 200; i++) {
                    countup++;
                    moveup++;
                    if (world[i] != '#' && forbidden(i) == false) break;
                }
            }

            
            //whichever takes less to do, execute

            if (countdown < countup || countdown == countup){ 
                if (((world[rdown]) == '#' || forbidden(rdown))){
                    blacklist(robot_index); 
                    if (driveMode(world[rup])) return drivemode;
                    if (destructable(world[rup])) {
                        if (world[rup-21] != '#' || forbidden(rup-21) == false) return up_destroy;
                        else {blacklist(rup); goto checkpoint;}
                    }
                    return up;  
                }  
                //keep going down until right is free
                do{   
                    if(driveMode(world[rdown])) return drivemode;   
                    if (destructable(world[rdown])) {
                        if (world[rdown+21] != '#' || forbidden(rdown+21) == false) return down_destroy;
                        else {blacklist(rdown); goto checkpoint;}
                    }           
                    return down;
                }
                while(world[rdown]!= '#' && forbidden(rdown) == false);  
                //move the amount of steps needed to the opening
                for (int k=0; k<movedown;k++){
                    if(driveMode(world[rright])) return drivemode;
                    if (destructable(world[rright])) {
                        if (world[rright+1] != '#' || forbidden(rright+1) == false) return right_destroy;
                        else {blacklist(rright); goto checkpoint;}
                    }
                    return right;
                }
            }

            if (countdown > countup){
                if (((world[rup]) == '#' || forbidden(rup))){
                    blacklist(robot_index); 
                    if (driveMode(world[rdown])) return drivemode;
                    if (destructable(world[rdown])) {
                        if (world[rdown+21] != '#' || forbidden(rdown+21) == false) return down_destroy;
                        else {blacklist(rdown); goto checkpoint;}
                    }
                    return down;  
                }  
                //keep going up until right is free
                do{    
                    if(driveMode(world[rup])) return drivemode;
                    if (destructable(world[rup])) {
                        if (world[rup-21] != '#' || forbidden(rup-21) == false) return up_destroy;
                        else {blacklist(rup); goto checkpoint;}
                    }
                    return up;
                }
                while(world[rup] != '#' && forbidden(rup) == false);
                
                //move the amount of steps needed to the opening 
                for (int i=0; i<moveup;i++){
                    if(driveMode(world[rright])) return drivemode;
                    if (destructable(world[rright])) {
                        if (world[rright+1] != '#' || forbidden(rright+1) == false) return right_destroy;
                        else {blacklist(rright); goto checkpoint;}
                    }
                    return right;
                }
            }
        }

       
        if (driveMode(world[rright])) return drivemode;
        if (destructable(world[rright])) {
                if (world[rright+1] != '#' || forbidden(rright+1) == false) return right_destroy;
                else {blacklist(rright); goto checkpoint;}
        }
        return right; 
        NoVertical=0;
    }


    //LEFT
    else if ((hR-hT) > 0 && NoHorizontal == false)  {
        printf("going left!\n");
        //if left blocked 
        if(world[rleft] == '#'|| forbidden(rleft)){
            if((world[rdown] != '#') || forbidden(rdown) != true) {
                movedown=0;
                int i;
                if (world[rdown- 1]  != '#' && forbidden((rdown-1)) == false) i = (rdown);
                else i = (rdown-1);                   
                //loop down left until an O is found
                for(i; i > 0; i--) {
                    movedown++;
                    countdown++;
                    if (world[i] != '#' &&  forbidden(i) == false) break;
                }
               printf("it took me %d steps down left to find an O!\n countdown: %d", movedown, countdown);
            }
            
            //if left is blocked: look for an opening after going up then left, and count how many steps
            if((world[rup] != '#') || forbidden(rup) != true) {
                moveup=0;
                int i;
                if (world[rup- 1]  != '#' && forbidden((rup-1)) == false) i = (rup);
                else i = (rup-1);                   
                //loop up then left until an O is found
                for(i; i > 0; i--) {  
                    moveup++;                      
                    countup++;
                    if (world[i] != '#' && forbidden(i) == false) break;
                }
                printf("it took me %d steps up left to find an O! \n countup: %d", moveup, countup);
            }


            if (countdown < countup || countdown == countup){
                if (((world[rdown]) == '#' || forbidden(rdown))){
                    blacklist(robot_index); 
                    if (driveMode(world[rup])) return drivemode;
                    if (destructable(world[rup])) {
                        if (world[rup-21] != '#' || forbidden(rup-21) == false) return up_destroy;
                        else {blacklist(rup); goto checkpoint;}
                    }
                    return up;  
                }  
                //keep going down until left is free
                do{            
                    if(driveMode(world[rdown])) return drivemode;   
                    if (destructable(world[rdown])) {
                        if (world[rdown+21] != '#' || forbidden(rdown+21) == false) return down_destroy;
                        else {blacklist(rdown); goto checkpoint;}
                    }
                    return down;
                }
                while(world[rdown] != '#' && forbidden(rdown) == false);
                //move the amount of steps needed to the opening
                for (int i=0; i<movedown;i++){
                    if(driveMode(world[rleft])) return drivemode;
                    //check if obstacle must be destroyed, destroy if yes
                    if (destructable(world[rleft])) {
                        if (world[rleft-1] != '#' || forbidden(rleft-1) == false) return left_destroy;
                        else {blacklist(rleft); goto checkpoint;}
                    }
                    return left;
                }
            }

            if (countdown > countup){
                if (((world[rup]) == '#'|| forbidden(rup))){
                    blacklist(robot_index); 
                    if (driveMode(world[rdown])) return drivemode;
                    if (destructable(world[rdown])) {
                        if (world[rdown+21] != '#' || forbidden(rdown+21) == false) return down_destroy;
                        else {blacklist(rdown); goto checkpoint;}
                    }
                    return down;  
                }                  
                //keep going up until left is free
                do{      
                    if(driveMode(world[rup])) return drivemode;    
                    if (destructable(world[rup])) {
                        if (world[rup-21] != '#' || forbidden(rup-21) == false) return up_destroy;
                        else {blacklist(rup); goto checkpoint;}
                    }            
                    return up;
                }
                while(world[rup] != '#' &&  forbidden(rup) == false);

                //move the amount of steps needed to the opening 
                for (int i=0; i<moveup;i++){
                    if(driveMode(world[rleft])) return drivemode;
                    //check if obstacle must be destroyed, destroy if yes
                    if (destructable(world[rleft])) {
                        if (world[rleft-1] != '#' || forbidden(rleft-1) == false) return left_destroy;
                        else {blacklist(rleft); goto checkpoint;}
                    }
                    return left;
                }
            }
                
        }

        // when no obstacles:
        if (driveMode(world[rleft])) return drivemode;
        //check if obstacle must be destroyed, destroy if yes
        if (destructable(world[rleft])) {
            if (world[rleft-1] != '#' || forbidden(rleft-1) == false) return left_destroy;
            else {blacklist(rleft); goto checkpoint;}
        }
        return left;
        NoVertical=0;
    }
    else{NoHorizontal = true;} //temporary horizontal lock

        //if deadlock occur
    if (NoVertical == true && NoHorizontal == true )
    {
        printf("\nSTUCK! Resetting!");
        NoVertical = 0;
        NoHorizontal = 0;
        goto checkpoint;
    }


}