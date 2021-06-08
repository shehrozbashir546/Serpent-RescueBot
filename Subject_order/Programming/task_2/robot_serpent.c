#include "robot_serpent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// IMPLEMET THIS FUNCTION
/* 1. find surroundings of T DONE
2. add # to closed list, add O to open list indexes DONE
3. calculate distance of each index in open list to R
4. shortest distance will stay in list, longest will go away
5. if index is in the closed array, dont go there
6. the path could be purely the order of the open list array, R just follows that path*/

// ALLOWED RETURN VALUES:
// 1: North, 2: East, 3: South, 4: West, 5: Toggle watern/land mode
int open_count=0; // amount of nodes in the open table
int closed_count=0; //amount of nodes in the closed table
int open_list[0]; //array to store all the open nodes indexes
int closed_list[0]; //array to store all closed node indexes

int safe(char *world,int robot_index, int target_index, int width, int elements) {
    int rup = robot_index - width - 1;
    int rdown = robot_index + width + 1;
    int rleft = robot_index - 1;
    int rright = robot_index + 1;

    int tup = target_index - width - 1;
    int tdown = target_index + width + 1;
    int tleft = target_index - 1;
    int tright = target_index + 1;
    int tsurround[4] = {tup,tright,tdown,tleft};    

    for (int i; i<4; i++){
        if (world[tsurround[i]] == '#'){
            for (int j=200; j>=0;j--) {
                closed_list[j] = closed_list[j-1];
                closed_list[0]=tsurround[i];
            }    
            closed_count++;
            /*int length = sizeof(closed_list)/sizeof(closed_list[0]);
            for (int j=0; j<=length;j++){
                printf("CLOSED LIST ELEMENTS: %d ", closed_list[j]);
            }   */
            
        }

        else if (world[tsurround[i]] == 'O'){
            for (int j=200; j>=0;j--) {
                open_list[j] = open_list[j-1];
                open_list[0]=tsurround[i];
            }   
            open_count++;
        }
    }
        int distance;
        int lines=0;
        int open_length = sizeof(open_list)/sizeof(open_list[0]);
        for (int j=0; j<=open_length;j++){ //for every element, calculate distance to R, and the smallest one will stay, rest deleted
            printf("OPEN LIST ELEMENTS: %d ", open_list[j]);
            distance= abs(robot_index-open_list[j]);
            /*
            1. make a function that checks which element of the array has the smallest (lines+distance)
            note: if they have the same distance, random choose 1.
            2. remove the rest of the open list items from the list.
            3. "move" the target to the only item in the list
            4. repeat the same process

            */
            if (robot_index < open_list[j]) { //if the open list item is bigger than the robot, calculate distance and amount of lines
                for(int i = robot_index; i < open_list[j]; ++i) { 
                    printf("TEST");
                    if (world[i] == '\n') { //how many lines between 
                        lines++;
                        printf("lines: %d", lines);
                    }
                }
            }
            else if (robot_index > open_list[j]) {
                    for(int i = open_list[j]; i < robot_index; ++i) { 
                    if (world[i] == '\n') {
                        printf("TEST3");
                        lines++;
                        printf("lines: %d", lines);
                    }
                }
            }
        }
    }


void pathLength(char *open_list){
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
    int tsurround[4] = {tup,tright,tdown,tleft};
    int rsurround[4] = {rup,rright,rdown,rleft};
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
        while (lines > 0 && rsurround[2] != '#') ; 
        
        //while R is on same line of T, go right.
        do {
            for (int i=0; i<distance;i++) {
	            return 2;
            }
        }
        while (lines < 1 && distance > 0 && rsurround[1] !='#');

        //while R is on the same line of T, go left.
        do  {
            for (int i=0; i<distance;i++) {
	            return 4;
            }
	    }
        while (lines < 1 && distance < 0 && rsurround[3] !='#');
    }

    //if T has smaller position index than R in the array
    else if (robot_index > target_index){
        //distance between indexes of R and T
        safe(world,robot_index,target_index, width, elements);
        int distance = robot_index - target_index;
        //count how many lines between R and T
        for(int i = target_index; i < robot_index; ++i) { 
            if (worldcpy[i] == '\n') {
                lines++;
            }
        }

        do { 
            for (int i=0; i<lines;i++) {
                return 1;
	        }       
        }
        while (lines > 0 && rsurround[0] !='#') ; 
    
        //while R is on the same line of T, go left
        do {
            for (int i=0; i<distance;i++) {
	            return 4;
            }
        }
        while (lines < 1 && distance > 0 && rsurround[3] !='#');

        //while R is on same line of T, go right.
        do  {
            for (int i=0; i<distance;i++) {
	            return 2;
            }
	    }
        while (lines < 1 && distance < 0 && rsurround[1] !='#');
    }
}