#include <stdio.h>
#include "robot_example.h"
#include <stdbool.h>

// IMPLEMENT THIS FUNCTION  g++ -o prog robo_world_3.c robot_example.c
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

int driveMode(char infront){
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
    char worldcpy[200];
    for(int i=0;i<200;i++) {
        
        worldcpy[i] =world[i];
    }
    
    unsigned int elements = sizeof(worldcpy)/sizeof(worldcpy[0]);
    int robot_index;
    int target_index;
    int X_index;
    int lines=0;
    int width=20;

    for(int i = 0; i < elements; ++i) { /*index of R*/ if (world[i] == 'R') { robot_index = i; break; }  }

    for(int i = 0; i < elements; ++i) { /*index of T*/ if (world[i] == 'T') {target_index = i; break; }  }

    int rup = robot_index - width - 1;
    int rdown = robot_index + width + 1;
    int rleft = robot_index - 1;
    int rright = robot_index + 1;
    int rsurround[4] = {rup,rright,rdown,rleft};

    int hT; //horizontal T
    int vT; //vertical T
    int hR; //horizontal R;
    int vR; //vertical R
    int hX; //horizontal X
    int vX; //vertical X
    //calculate horizontal and vertical T position
    hT = target_index % 21;
    vT = (target_index / 21) + 2;
    //calculate horizontal and vertical R position
    hR = robot_index % 21;
    vR = (robot_index / 21) + 2;
    int countdown=0;
    int countup=0;
    int countleft=0;
    int countright=0;
    printf("The index R is %d, T is %d\n",robot_index,target_index);
    int forwardlock = 1;

    for(int i=0;i<200;i++){
        if((target_index>0) && (target_index<200)) {forwardlock=1; }
        else{/*printf("T is not in the map!\n");*/      forwardlock=0;}
    }
    if(forwardlock==0) {
        for(int i = 0; i < elements; ++i) { /*index of T*/ if (world[i] == 'X') {X_index = i; break; }  }
        hX = X_index % 21;
        vX = (X_index / 21) + 2;


    }


//Green Code
if(forwardlock){
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
            else if (world[rright] != '#' || forbidden(rright) == false) {
                //loop goes on till the first O is found starting right up
                for( int i =(rright - width); i < 1; (i-width-1)) {
                    countright++;
                    if (world[i] == 'O') break;
                }
            }

            else if (world[rleft] != '#' || forbidden(rleft) == false) {
                 for( int i =(rleft - width - 2); i <1; (i-width-1)) {
                        countleft++;
                        if (world[i] == 'O') break;
                    }
            }
            
            //if it takes less steps to go left, or its the same as right, go left then up
            if (countleft < countright || countleft == countright) {
                //go left until up is clear
                do{
                    if (driveMode(world[rleft]))return drivemode;
                    printf("Left is working!\n");
                    return left;
                }
                while(world[rleft] == 'O' || world[rleft] == '~');
                //move up according to count
                for (int i=0; i<countleft;i++){
                    return up;
                }
            }//g++ -o prog robo_world_3.c robot_example.c
            //if it takes less steps to go right then up, do it
            if (countleft > countright ) {
                //go right until up is clear
                do{ 
                    if (driveMode(world[rright])) return drivemode;
                    printf("Right drive mode is working\n");
                    return right;
                }
                while(world[rright] == 'O' || world[rright] == '~');
                    
                for (int i=0; i<countright;i++){
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
                for(int i =rright+width+1; i > 200; (i+width+1)) {
                    countright++;
                    if (world[i] == 'O') break;
                }
            }

            if (world[rleft] != '#' || forbidden(rleft) == false) {
                for(int i =(rleft+width+1); i > 200; (i+width+1)) {
                    countright++;
                    if (world[i] == 'O') break;
                }
            }
            

            if (countleft < countright || countleft == countright) {
                do{
                    if (driveMode(world[rleft])) return drivemode;
                    return 4;
                }
                while((world[rleft] == 'O' || world[rleft] == '~') && forbidden(rleft) ==false);
                    
                for (int i=0; i<countleft;i++){
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
                    while((world[rup] == '#' || forbidden(rup)) && (world[rright] == 'O' || world[rright] == '~'));
                        
                    for (int i=0; i<countright;i++){
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
                countdown=0;
                for( int i = rdown+1; i > 200; i++) {
                    countdown++;
                    if (world[i] == 'O') break;
                }
                
            }

            if(world[rup] != '#' || forbidden(rup) != true) {
                countup=0;
                for( int i = rup+1; i  > 200; i++) {
                    countup++;
                    if (world[i] == 'O') break;
                }
            }

            
            //whichever takes less to do, execute

            if (countdown < countup || countdown == countup){
                //keep going down until right is free
                do{  
                    if(driveMode(world[rdown])) return drivemode;                      
                    return down;
                }
                    while(world[rdown] == 'O' || world[rdown] == '~');
                    //move the amount of steps needed to the opening
                    for (int i=0; i<countdown;i++){
                        if(driveMode(world[rright])) return drivemode;
                        return right;
                    }
                }

            else if (countdown > countup){
                //keep going up until right is free
                do{    
                    if(driveMode(world[rup])) return drivemode;
                    return up;
                }
                while(world[rleft] == '#' && world[rup] == 'O');
                
                //move the amount of steps needed to the opening 
                for (int i=0; i<countup;i++){
                    if(driveMode(world[rright])) return drivemode;
                    return right;
                }
            }
        }

        NoVertical=0;
        if (driveMode(world[rright])) return drivemode;
        return right;
    }


    //LEFT //why is there an else!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    else if ((hR-hT) > 0 && noHorizontal == 0)  {
        printf("going left!\n");
        //if left blocked 
        if(world[rleft]=='#' || forbidden(rleft)){
            //check if its blocked from up and down, add to blacklist if yes.
            if ((world[rup] == '#' || forbidden(rup)) && (world[rdown]=='#' || forbidden(rdown))){ 
                blacklist(robot_index);
                if (driveMode(world[rright])) return drivemode; //changing to rright
                printf("sketchy code 1\n");
                return right;
            }

 
            if(world[rdown] != '#' || forbidden(rdown) != true) {
                //loop down left until an O is found
                for( int i = rdown-1; i > 0; i--) {
                    countdown++;
                    if (world[i] == 'O') break;
                }
               // printf("it took me %d steps down left to find an O!", countdown);
            }
            
            //if left is blocked: look for an opening after going up then left, and count how many steps
            if(world[rup] != '#' || forbidden(rup) != true) {
                //loop up then left until an O is found
                for( int i = rup-1; i > 0; i--) {                        
                    countup++;
                    if (world[i] == 'O') break;
                }
                //printf("it took me %d steps up left to find an O!", countup);
            }


            if (countdown < countup || countdown == countup){
                //keep going down until left is free
                do{                  
                    if(driveMode(world[rdown])) return drivemode;      
                        printf("sketchy code 69\n");
                        return down;
                }
                while(world[rdown] == 'O' || world[rdown] == '~');

                //move the amount of steps needed to the opening
                for (int i=0; i<countdown;i++){
                    if(driveMode(world[rleft])) return drivemode;
                    printf("sketchy code IMPORTANT\n");
                    return left;
                }
            }

            else if (countdown > countup){  /////changeD TO ELSE!!!!!!!!!!
                //keep going up until left is free
                do{      
                    if(driveMode(world[rup])) return drivemode;                  
                    return up;
                }
                while(world[rup] == '#' || world[rup] == 'O'); //CHANGED FROM O TO HASH WATER TO O

                //move the amount of steps needed to the opening 
                for (int i=0; i<countup;i++){
                    if(driveMode(world[rleft])) return drivemode;
                    printf("This weird code is running!\n");
                    return left;
                }
            }
                
        }
            // when no obstacles:
            NoVertical=0;
            if (driveMode(world[rleft])) return drivemode; //RRIGHT TO RLEFT
            return left;
    }

            else{noHorizontal = true;} //temporary horizontal lock

    

 
    
    //if deadlock occur
    if (NoVertical == true && noHorizontal == true )
    {
        printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        NoVertical = 0;
        noHorizontal = 0;
        goto checkpoint;
    }

}
//Blue Code
    else {
    
       checkpoint2:
    //if T has bigger y position index than R in the array
    
            if((vR-vX) > 0 && NoVertical == false){ 
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
            else if (world[rright] != '#' || forbidden(rright) == false) {
                //loop goes on till the first O is found starting right up
                for( int i =(rright - width); i < 1; (i-width-1)) {
                    countright++;
                    if (world[i] == 'O') break;
                }
            }

            else if (world[rleft] != '#' || forbidden(rleft) == false) {
                 for( int i =(rleft - width - 2); i <1; (i-width-1)) {
                        countleft++;
                        if (world[i] == 'O') break;
                    }
            }
            
            //if it takes less steps to go left, or its the same as right, go left then up
            if (countleft < countright || countleft == countright) {
                //go left until up is clear
                do{
                    if (driveMode(world[rleft]))return drivemode;
                    return left;
                }
                while(world[rleft] == 'O' || world[rleft] == '~');
                //move up according to count
                for (int i=0; i<countleft;i++){
                    return up;
                }
            }//g++ -o prog robo_world_3.c robot_example.c
            //if it takes less steps to go right then up, do it
            if (countleft > countright ) {
                //go right until up is clear
                do{ 
                    if (driveMode(world[rright])) return drivemode;
                    return right;
                }
                while(world[rright] == 'O' || world[rright] == '~');
                    
                for (int i=0; i<countright;i++){
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
            if ((vR-vX) < 0 && NoVertical == false){  
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
                for(int i =rright+width+1; i > 200; (i+width+1)) {
                    countright++;
                    if (world[i] == 'O') break;
                }
            }

            if (world[rleft] != '#' || forbidden(rleft) == false) {
                for(int i =(rleft+width+1); i > 200; (i+width+1)) {
                    countright++;
                    if (world[i] == 'O') break;
                }
            }
            

            if (countleft < countright || countleft == countright) {
                do{
                    if (driveMode(world[rleft])) return drivemode;
                    return 4;
                }
                while((world[rleft] == 'O' || world[rleft] == '~') && forbidden(rleft) ==false);
                    
                for (int i=0; i<countleft;i++){
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
                    while((world[rup] == '#' || forbidden(rup)) && (world[rright] == 'O' || world[rright] == '~'));
                        
                    for (int i=0; i<countright;i++){
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
            if((hR-hX) < 0 && noHorizontal == false) {
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
                countdown=0;
                for( int i = rdown+1; i > 200; i++) {
                    countdown++;
                    if (world[i] == 'O') break;
                }
                
            }

            if(world[rup] != '#' || forbidden(rup) != true) {
                countup=0;
                for( int i = rup+1; i  > 200; i++) {
                    countup++;
                    if (world[i] == 'O') break;
                }
            }

            
            //whichever takes less to do, execute

            if (countdown < countup || countdown == countup){
                //keep going down until right is free
                do{  
                    if(driveMode(world[rdown])) return drivemode;                      
                    return down;
                }
                    while(world[rdown] == 'O' || world[rdown] == '~');
                    //move the amount of steps needed to the opening
                    for (int i=0; i<countdown;i++){
                        if(driveMode(world[rright])) return drivemode;
                        return right;
                    }
                }

            else if (countdown > countup){
                //keep going up until right is free
                do{    
                    if(driveMode(world[rup])) return drivemode;
                    return up;
                }
                while(world[rleft] == '#' && world[rup] == 'O');
                
                //move the amount of steps needed to the opening 
                for (int i=0; i<countup;i++){
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
            else if ((hR-hX) > 0 && noHorizontal == 0)  {
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
                //loop down left until an O is found
                for( int i = rdown-1; i > 0; i--) {
                    countdown++;
                    if (world[i] == 'O') break;
                }
               // printf("it took me %d steps down left to find an O!", countdown);
            }
            
            //if left is blocked: look for an opening after going up then left, and count how many steps
            if(world[rup] != '#' || forbidden(rup) != true) {
                //loop up then left until an O is found
                for( int i = rup-1; i > 0; i--) {                        
                    countup++;
                    if (world[i] == 'O') break;
                }
                //printf("it took me %d steps up left to find an O!", countup);
            }


            if (countdown < countup || countdown == countup){
                //keep going down until left is free
                do{                  
                    if(driveMode(world[rdown])) return drivemode;      
                    return down;
                }
                while(world[rdown] == 'O' || world[rdown] == '~');

                //move the amount of steps needed to the opening
                for (int i=0; i<countdown;i++){
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
                while(world[rup] == 'O' || world[rup] == '~');

                //move the amount of steps needed to the opening 
                for (int i=0; i<countup;i++){
                    if(driveMode(world[rleft])) return drivemode;
                    return left;
                }
            }
                
        }
            // when no obstacles:
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
        goto checkpoint2;
    }
    }
}