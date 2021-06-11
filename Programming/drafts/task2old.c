#include "robot_serpent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ALLOWED RETURN VALUES:
// 1: North, 2: East, 3: South, 4: West, 5: Toggle watern/land mode

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
            if (world[i] == '\n') {
                lines++;
            }
        }

        //DOWN
        if (lines > 0){  
            printf("DOWN %d", lines);

            if (world[rdown]=='#'){
                // if there is obstacle below R, and the left is free, check how many steps it would take to go left and then down
                if(world[rdown] == '#' && world[rleft] == 'O'){
                    for(int i =rleft + width; i >= elements; i+width+1) {
                        countleft++;
                        if (world[i] == 'O') break;
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

            if (world[rdown] == 'O') {
                return 3;
            }
        }
        
        //RIGHT
        do {printf("RIGHT but R<T %d", lines);


            if (world[rright] == '#'){
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
            if(world[rright]=='O'){
                 return 2;
            }
        }
        while (lines < 1 && distance > 0);

        //LEFT
        do  {printf("LEFT but R<T %d", lines); 

            if (world[rleft] == '#'){
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
            if (world[rleft]=='O'){
                    return 4;
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

        //UP
        if(lines>0) { 
            printf("UP %d\n", lines);
            //go up until there is obstacle above R

            if (world[rup] == '#'){
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
            if (world[rup] == 'O') {
                 return 1;
            }

        }

      

        //LEFT
        do {
            printf("LEFT %d\n", lines);

            if (world[rleft]=='#'){
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
            if (world[rleft]=='O'){
                return 4;
            }
        }
        while (lines ==0 && distance > 0);

        //RIGHT
        do  {printf("RIGHT %d\n", lines);

           if (world[rright]=='#'){
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
                        if (world[rright]== 'O'){
                return 2;
            }
	    }
        while (lines ==0 && distance < 0);
    }
}