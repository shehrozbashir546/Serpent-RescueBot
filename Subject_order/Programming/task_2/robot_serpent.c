#include "robot_serpent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// IMPLEMET THIS FUNCTION
/* 1. find surroundings of T DONE
2. add # to closed list, add O to open list indexes DONE
3. calculate distance of each index in open list to R DONE
4. shortest distance will stay in list, longest will go away
5. if index is in the closed array, dont go there
6. the path could be purely the order of the open list array, R just follows that path*/

// ALLOWED RETURN VALUES:
// 1: North, 2: East, 3: South, 4: West, 5: Toggle watern/land mode
int open_count=0; // amount of nodes in the open table
int closed_count=0; //amount of nodes in the closed table
int open_list[20]; //array to store all the open nodes indexes
int closed_list[20]; //array to store all closed node indexes


int path[10];
int distance;
int lines=0;
int open_length = sizeof(open_list)/sizeof(open_list[0]); //length of open list array
int steps;
int map_width=21; //width with new lines
/*int check(int open_count,char *world, int target_index, int width, int fake_target, int elem){
    if (open_count == 1){
        fake_target++;
        safe(world, target_index,width,elements);
    }
}
int safe(char *world, int target_index, int width, int elements) {
    

    int fake_target=target_index;
    int ftup = fake_target - width - 1;
    int ftdown = fake_target + width + 1;
    int ftleft = fake_target - 1;
    int ftright = fake_target + 1;
    int ftsurround[4] = {ftup,ftright,ftdown,ftleft};    
    for (int i; i<4; i++){          
        if (world[ftsurround[i]] == '#'){
            for (int j=200; j>=0;j--) {
                closed_list[j] = closed_list[j-1];
                closed_list[0]=ftsurround[i];
            }    
            closed_count++;
            /*int length = sizeof(closed_list)/sizeof(closed_list[0]);
            for (int j=0; j<=length;j++){
            //}        
        }

        else if (world[ftsurround[i]] == 'O' || world[ftsurround[i]] == '~'){
            for (int j=200; j>=0;j--) {
                open_list[j] = open_list[j-1];
                open_list[0]=ftsurround[i];   
            }     
            open_count++;

        }
    } 
    int empty = open_list[0];
    fake_target=empty;

    printf("\nfake target: %d",fake_target); 
    fake_target++;    
}*/


        /*
        1. make a function that checks which element of the array has the smallest (lines+distance)
        note: if they have the same distance, random choose 1.
        2. remove the rest of the open list items from the list.
        3. "move" the target to the only item in the list
        4. repeat the same process
        */
   
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
        if (worldcpy[i] == 'R') {
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
    
    
    
    int tup = target_index - width - 1;
    int tdown = target_index + width + 1;
    int tleft = target_index - 1;
    int tright = target_index + 1;
    int tsurround[4] = {tup,tright,tdown,tleft}; 

    int countdown=0;
    int countup=0;
    int countleft=0;
    int countright=0;
    //if T has bigger position index than R in the array
    if(robot_index < target_index){ 
        //distance between indexes of R and T
        int distance = target_index - robot_index;	 

        //count how many lines between R and T
        for(int i = robot_index; i < target_index; ++i) { 
            if (worldcpy[i] == '\n') {
                lines++;
            }
        }
        //as long as R is below T, go down, until it reaches the same line.
        do {  
            if (world[rdown] == 'O') {
                for (int i=0; i<lines;i++) {return 3;}
            }

            // if there is obstacle below R, and the left is free, check how many steps it would take to go left and then down
            if(world[rdown] == '#' && world[rleft] == 'O'){
                for(int i =(rleft + width); i >= elements; (i+width+1)) {
                    countleft++;
                    if (world[i] == 'O') {
                    break;
                    }
                }
            }
            //same but for right
            if(world[rdown] == '#' && world[rright] == 'O'){
                for(int i =(rleft + width +2); i >= elements; (i+width+1)) {
                    countright++;
                    if (world[i] == 'O') {
                    break;
                    }
                }
            }

            //if it takes less steps to go left, or its the same as right, go left then down
            if (countleft < countright || countleft == countright) {
                do{return 4;}
                while(world[rdown] == '#' && world[rleft] == 'O');

                for (int i=0; i<countleft;i++){
                    return 3;
                }
            }
            //if it takes less steps to go right then down, do it
            if (countleft > countright ) {
                do{return 2;}
                while(world[rup] == '#' && world[rright] == 'O');
                for (int i=0; i<countright;i++){
                    return 3;
                }
            }
        }
        while (lines > 0) ; 
        
        //while R is on same line of T, go right.
        do { 
                
                if(world[rright]=='O'){
                    for (int i=0; i<distance;i++) { return 2;}
                }
                //if right is blocked: look for an opening after going down and right, and count how many steps
                if(world[rright] == '#' && world[rdown] == 'O'){
                    //loop down right until an O is found
                    for( int i = rdown+1; i <= width; i++) {
                        countdown++;
                        if (world[i] == 'O') {
                        break;
                        }
                    }
                   // printf("it took me %d steps down right to find an O!", countdown);
                }

                //if right is blocked: look for an opening after going up then right, and count how many steps
                if(world[rright] == '#' && world[rup] == 'O'){
                    //loop dup then right until an O is found
                    for( int i = rup+1; i <= width; i++) {
                        countup++;
                        if (world[i] == 'O') {
                        break;
                        }
                    }
                    //printf("it took me %d steps up right to find an O!", countup);
                }
                
                //whichever takes less to do, execute

                if (countdown < countup || countdown == countup){
                    //keep going down until right is free
                    do{                        
                        return 3;
                    }
                    while(world[rright] == '#' && world[rdown] == 'O');

                    //move the amount of steps needed to the opening
                    for (int i=0; i<countdown;i++){
                        return 2;
                    }
                }

                if (countdown > countup){
                    //keep going up until right is free
                    do{                        
                        return 1;
                    }
                    while(world[rright] == '#' && world[rup] == 'O');

                    //move the amount of steps needed to the opening 
                    for (int i=0; i<countup;i++){
                        return 2;
                    }
                }
            
        }
        while (lines < 1 && distance > 0);

        //while R is on the same line of T, go left.
        do  {
                for (int i=0; i<distance;i++) { 
                //if left is blocked: look for an opening after going down and left, and count how many steps
                if(world[rleft] == '#' && world[rdown] == 'O'){
                    //loop down left until an O is found
                    for( int i = rdown-1; i <= width; i++) {
                        countdown++;
                        if (world[i] == 'O') {
                        break;
                        }
                    }
                   // printf("it took me %d steps down left to find an O!", countdown);
                }

                //if left is blocked: look for an opening after going up then left, and count how many steps
                if(world[rleft] == '#' && world[rup] == 'O'){
                    //loop dup then left until an O is found
                    for( int i = rup-1; i <= width; i++) {
                        countup++;
                        if (world[i] == 'O') {
                        break;
                        }
                    }
                    //printf("it took me %d steps up left to find an O!", countup);
                }
                
                //whichever takes less to do, execute

                if (countdown < countup || countdown == countup){
                    //keep going down until left is free
                    do{                        
                        return 3;
                    }
                    while(world[rleft] == '#' && world[rdown] == 'O');

                    //move the amount of steps needed to the opening
                    for (int i=0; i<countdown;i++){
                        return 4;
                    }
                }

                if (countdown > countup){
                    //keep going up until left is free
                    do{                        
                        return 1;
                    }
                    while(world[rleft] == '#' && world[rup] == 'O');

                    //move the amount of steps needed to the opening 
                    for (int i=0; i<countup;i++){
                        return 4;
                    }
                }
            }
	    }
        while (lines < 1 && distance < 0);
    }

    //if T has smaller position index than R in the array
    else if (robot_index > target_index){
        //distance between indexes of R and T
       // safe(world,target_index, width, elements);
        int distance = robot_index - target_index;
        //count how many lines between R and T
        for(int i = target_index; i < robot_index; ++i) { 
            if (worldcpy[i] == '\n') {
                lines++;
            }
        }

        //go up
        do { 
            printf("HELLO %d\n", lines);
            //go up until there is obstacle above R
            if (world[rup] == 'O') {
                for (int i=0; i<lines;i++) {return 1;}
            }

            // if there is obstacle above R, and the left is free, check how many steps it would take to go left and then up
            if(world[rup] == '#' && world[rleft] == 'O'){
                for( int i =(rleft - width - 2); i >= elements; (i-width-1)) {
                    countleft++;
                    if (world[i] == 'O') {
                    break;
                    }
                }
            }
            //same but for right
            if(world[rup] == '#' && world[rright] == 'O'){
                for( int i =(rright - width); i >= elements; (i-width-1)) {
                    countright++;
                    if (world[i] == 'O') {
                    break;
                    }
                }
            }

            //if it takes less steps to go left, or its the same as right, go left then up
            if (countleft < countright || countleft == countright) {
                do{return 4;}
                while(world[rup] == '#' && world[rleft] == 'O');

                for (int i=0; i<countleft;i++){
                    return 1;
                }
            }
            //if it takes less steps to go right then up, do it
            if (countleft > countright ) {
                do{return 2;}
                while(world[rup] == '#' && world[rright] == 'O');
                for (int i=0; i<countright;i++){
                    return 1;
                }
            }
        }
        while (lines > 0) ; 

      

        //while R is on the same line of T, go left
        do {
            for (int i=0; i<distance;i++) { 
                //if left is blocked: look for an opening after going down and left, and count how many steps
                if(world[rleft] == '#' && world[rdown] == 'O'){
                    //loop down left until an O is found
                    for( int i = rdown-1; i >= width; i--) {
                        countdown++;
                        if (world[i] == 'O') {
                        break;
                        }
                    }
                   // printf("it took me %d steps down left to find an O!", countdown);
                }

                //if left is blocked: look for an opening after going up then left, and count how many steps
                if(world[rleft] == '#' && world[rup] == 'O'){
                    //loop dup then left until an O is found
                    for( int i = rup-1; i >= width; i--) {
                        countup++;
                        if (world[i] == 'O') {
                        break;
                        }
                    }
                    //printf("it took me %d steps up left to find an O!", countup);
                }
                
                //whichever takes less to do, execute

                if (countdown < countup || countdown == countup){
                    //keep going down until left is free
                    do{                        
                        return 3;
                    }
                    while(world[rleft] == '#' && world[rdown] == 'O');

                    //move the amount of steps needed to the opening
                    for (int i=0; i<countdown;i++){
                        return 4;
                    }
                }

                if (countdown > countup){
                    //keep going up until left is free
                    do{                        
                        return 1;
                    }
                    while(world[rleft] == '#' && world[rup] == 'O');

                    //move the amount of steps needed to the opening 
                    for (int i=0; i<countup;i++){
                        return 4;
                    }
                }
            }
        }
        while (lines < 1 && distance > 0);

        //while R is on same line of T, go right.
        do  {
            if (world[rright]== 'O'){
                for (int i=0; i<distance;i++) {return 2;}
            }
            //if rright is blocked: look for an opening after going down and right, and count how many steps
                if(world[rright] == '#' && world[rdown] == 'O'){
                    //loop down right until an O is found
                    for( int i = rdown+1; i <= width; i++) {
                        countdown++;
                        if (world[i] == 'O') {
                        break;
                        }
                    }
                   // printf("it took me %d steps down right to find an O!", countdown);
                }

                //if right is blocked: look for an opening after going up then right, and count how many steps
                if(world[rright] == '#' && world[rup] == 'O'){
                    //loop dup then right until an O is found
                    for( int i = rup+1; i <= width; i++) {
                        countup++;
                        if (world[i] == 'O') {
                        break;
                        }
                    }
                    //printf("it took me %d steps up right to find an O!", countup);
                }
                
                //whichever takes less to do, execute

                if (countdown < countup || countdown == countup){
                    //keep going down until right is free
                    do{                        
                        return 3;
                    }
                    while(world[rright] == '#' && world[rdown] == 'O');

                    //move the amount of steps needed to the opening
                    for (int i=0; i<countdown;i++){
                        return 2;
                    }
                }

                if (countdown > countup){
                    //keep going up until right is free
                    do{                        
                        return 1;
                    }
                    while(world[rleft] == '#' && world[rup] == 'O');

                    //move the amount of steps needed to the opening 
                    for (int i=0; i<countup;i++){
                        return 2;
                    }
                }
	    }
        while (lines < 1 && distance < 0);
    }
}