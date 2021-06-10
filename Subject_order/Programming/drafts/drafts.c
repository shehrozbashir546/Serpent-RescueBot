// IMPLEMET THIS FUNCTION
/* 1. find surroundings of T DONE
2. add # to closed list, add O to open list indexes DONE
3. calculate distance of each index in open list to R DONE
4. shortest distance will stay in list, longest will go away
5. if index is in the closed array, dont go there
6. the path could be purely the order of the open list array, R just follows that path*/
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