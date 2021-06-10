#include "robot_example.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<stdbool.h>
// 1: North, 2: East, 3: South, 4: West, 5: Toggle watern/land mode

int fridge; 
int loopLock = 0;
int cookie = 0;




int move(char *world) {
    //Initialize and find the locational indexes
    int RoLocation,TargetLocation, forr;
    for(forr=0;forr<200;forr++) {if(world[forr]=='R'){RoLocation=forr;}}
    for(forr=0;forr<200;forr++) {if(world[forr]=='T'){TargetLocation=forr;}}

    //printf("Got the location baus: R %d;T %d.\n",RoLocation,TargetLocation);
    //
    //Find the distance between Target and Robot
    //Do this by imagining the map as an x y axis
    int RyCoordinate = (RoLocation / 21) + 2;
    int RxCoordinate = RoLocation%21;
    int TyCoordinate = (TargetLocation / 21) + 2;
    int TxCoordinate = TargetLocation%21;

    //ZoneTime();
    for(int x =0;x<200;x++){
        if(world[x]=='#' && world[x+1]=='O' && world[x+2]=='#') {
            //printf("I exist!!\nMy Index Position is %d\n",x+2);
            fridge = x+2;
            
        }
    }
    //printf("Value of RoLo and fridge is %d,%d\n",RoLocation,fridge);
    
    // Index position of mouth is 93
    // B is at 113 20 from A
    int mouth = fridge+20;
    //Lock for the loop
    if(RoLocation<mouth) {loopLock=1; cookie=1;}
    if(RoLocation>fridge && RoLocation!=fridge && loopLock==0) {
        int distance = RoLocation-mouth;
        printf("i shouldnt be workin\n");
        int lines=0,width=20;
         //Find out how many lines are there b/w robot and target
        for(int l=mouth ; l<RoLocation; ++l) {
            if(world[l]=='\n') {
            lines++;}
        }
        //printf("line diff = %d\n",lines);
        // go up if there is no line difference
        if(lines>=1){
            return 1;
        }
        //go left 
        else if(distance>0 && lines==0 && distance<width) {
            return 4;
        }

        else if(distance<0 && lines==0 && distance<width) {
            return 2;
        }

        else {
            printf("This is over!\n");
            loopLock = 1;
            return 1;
        }
        
        
    }
    //this is quite important and will be used!
    //Now we are the mouth and we will find a way to traverse the maze
    /*if(world[RoLocation-20]!='#' || world[RoLocation-21]!='#'  && cookie==0) {
        printf("2nd if is running!\n");
        return 1;
    }
    else if(world[RoLocation-1]!='#' && cookie==0) {
        printf("2nd 2nd one if is running!\n");
        return 4;
    }
    else if(world[RoLocation+1]!='#'&& cookie==0) {
        printf("2nd if 3rd is running!\n");
        return 2;
    }*/





    if(RoLocation<mouth && RoLocation!=fridge && cookie==1) {
        printf("blah The lesser than func is working! \nFridge is %d,Mouth is %d\n",fridge,mouth);
        int distance = mouth-RoLocation;
        signed int lines=0,width=20;
         //Find out how many lines are there b/w robot and mouth
        for(int l=RoLocation ; l<mouth; ++l) {
            if(world[l]=='\n') {
            lines++;}
            else{lines==0;}
        }
        printf("line diff = %d, Distance = %d, Rolo is %d\n",lines,distance,RoLocation);
        // go down if there is a line difference
        if(lines>=1){
            printf("1st if ran\n");
            return 3;
        }
        
        //go right 
        else if((distance>0) && lines==0 && distance<width) {
            printf("2nd one ran \n");
            
            return 2;
        }

        else if(distance<0 && lines==0 && distance<width) {
            printf("3rd one ran ");
            return 4;
        }

        else if(distance==1){
            printf("This is over!\n");
            //cookie = 0;
            return 1;
        }

    }


    //i dont think ill use this    
  /*  else /*if(RoLocation==fridge){
        return 1;
        if(world[RoLocation-20]=='O') {
            return 1;
        }
        //printf("Line distance is %d\ndistance is %d\n",lines,distance); //printf("ROlOC %d\n",RoLocation);
    }*/
    
    
}

void excesscode() {
    //printf("The xy of R is %d,%d. The xy of T is %d,%d.\n",RxCoordinate,RyCoordinate,TxCoordinate,TyCoordinate);
    
    //Assume the robot is ahead of the target
  /*  if(TyCoordinate-RyCoordinate<0 && world[RoLocation-20!='#']){
        //it means the bot should go up
        int RyCoordinate = (RoLocation / 21) + 2;
        int TyCoordinate = (TargetLocation / 21) + 2;
        printf("The coordinates or R,T are %d,%d\n",RyCoordinate,TyCoordinate);
        return 1;
    }
    else if(TyCoordinate==RyCoordinate && world[RoLocation-1]=='O'){
        return 4;
    }
    else if(world[RoLocation-1]=='#' && world[RoLocation+20=='O']){
        //try going down
        return 3;
    }
    //lets check the area around T and find a path
    
    if(world[TargetLocation-1]=='#') {//Behind of T
        NoGoZones(TargetLocation-1);
    }
    if(world[TargetLocation+1]=='#') {//Front of T
        NoGoZones(TargetLocation+1);
    }
    if(world[TargetLocation+2]=='#') {//2 steps ahead of T
        NoGoZones(TargetLocation+2);
    }
    if(world[TargetLocation-2]=='#') {//2 steps behidn T
        NoGoZones(TargetLocation-2);
    }
    if(world[TargetLocation+20]=='#') {//below T
        NoGoZones(TargetLocation+20);
    }
    if(world[TargetLocation-20]=='#') {//above T
        NoGoZones(TargetLocation-20);
    }
    if(world[TargetLocation+3]=='#') {//3 steps ahead of T
        NoGoZones(TargetLocation+3);
    }*/
}