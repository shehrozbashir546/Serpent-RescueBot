#include "robot_serpent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// IMPLEMET THIS FUNCTION
// ALLOWED RETURN VALUES:
// 1: North, 2: East, 3: South, 4: West, 5: Toggle watern/land mode
/* idea: if statements, lets say R is going north then the index would check if R+width+1 = #, if yes, go right, do same before going right blah blah*/
int safe(char *world,int robot_index, int target_index, int width) {
    int rup = robot_index - width - 1;
    int rdown = robot_index + width + 1;
    int rleft = robot_index - 1;
    int rright = robot_index + 1;

    int tup = target_index - width - 1;
    int tdown = target_index + width + 1;
    int tleft = target_index - 1;
    int tright = target_index + 1;

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
        if (worldcpy[i] == 'R') {
            robot_index = i;
            break;
        }
    }

    for(int i = 0; i < elements; ++i) { //index of T
        if (worldcpy[i] == 'T') {
            target_index = i;
            break;
        }
    }
    int rup = robot_index - width - 1;
    int rdown = robot_index + width + 1;
    int rleft = robot_index - 1;
    int rright = robot_index + 1;

    int tup = target_index - width - 1;
    int tdown = target_index + width + 1;
    int tleft = target_index - 1;
    int tright = target_index + 1;
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

        //as long as R is above T, go down, until it reaches the same line.
        do {  
            for (int i=0; i<lines;i++) {
                return 3;
	        }       
        }
        while (lines > 0 && rdown !='#') ; 
        
        //while R is on same line of T, go right.
        do {
            for (int i=0; i<distance;i++) {
	            return 2;
            }
        }
        while (lines < 1 && distance > 0 && rright !='#');

        //while R is on the same line of T, go left.
        do  {
            for (int i=0; i<distance;i++) {
	            return 4;
            }
	    }
        while (lines < 1 && distance < 0 && rleft !='#');
    }

    //if T has smaller position index than R in the array
    else if (robot_index > target_index){
        //distance between indexes of R and T
        safe(worldcpy,robot_index, target_index, width);
        int distance = robot_index - target_index;
        //count how many lines between R and T
        for(int i = target_index; i < robot_index; ++i) { 
            if (worldcpy[i] == '\n') {
                lines++;
            }
        }

        do { 
            for (int i=0; i<lines;i++) {
                safe(worldcpy,robot_index,target_index,width);
                return 1;
	        }       
        }
        while (lines > 0) ; 
    
        //while R is on the same line of T, go left
        do {
            for (int i=0; i<distance;i++) {
	            return 4;
            }
        }
        while (lines < 1 && distance > 0);

        //while R is on same line of T, go right.
        do  {
            for (int i=0; i<distance;i++) {
	            return 2;
            }
	    }
        while (lines < 1 && distance < 0);
    }
}