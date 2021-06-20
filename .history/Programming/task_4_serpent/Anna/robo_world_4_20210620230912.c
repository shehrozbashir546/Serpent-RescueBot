/*
A very simple text based simulation of a rescue scenario.

Copyright 2021 Kristian Rother (kristian.rother@hshl.de)

Redistribution and use in source and binary forms, with or without modification, are permitted provided
that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions
and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to
endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "robot_serpent.h"

int driving_mode = 0;
int rescued = 0;
int energy = 0;
const int ENERGY_MOVE = 10;
const int ENERGY_TOGGLE = 30;
const int ENERGY_DESTROY = 70;
char coming_from = 'X'; // We start on the space we need to return to
char facing;

// THIS IS THE FUNCTION YOU IMPLEMENT
int move(char *world, int map_id);

void reset_world() {
    driving_mode = 0;
    rescued = 0;
    energy = 0;
    coming_from = 'X'; // We start on the space we need to return to
}

// Some ugly hardcoding of the height
void print_world(char *world, int size) {
    for(int i = 0; i< (size+1)*8; i++) {
        if(world[i] == 'O') { printf("%c", 'O'); }
        else { printf("%c", world[i]); }
    }
    printf("\n");
}

/*
void print_world(char *world) {
    printf("%s", world);
}
*/

// Return target index
int update_world(int movement, char *world, int robot_index, int width) {
    int target_index = 0;
    // NORTH
    if(movement == 1) {
        energy += ENERGY_MOVE;
        target_index = robot_index - (width + 1); // +1 for the newline
    }
    // SOUTH
    else if(movement == 3) {
        energy += ENERGY_MOVE;
        target_index = robot_index + (width + 1); // +1 for the newline
    }
    // EAST
    else if(movement == 2) {
        energy += ENERGY_MOVE;
        target_index = robot_index+1;
    }
    // WEST
    else if(movement == 4) {
        energy += ENERGY_MOVE;
        target_index = robot_index-1;
    }
    else if(movement == 5) {
        energy += ENERGY_TOGGLE;
        printf("Toggling mode!\n");
        target_index = robot_index;
        if(driving_mode == 0) { driving_mode = 1; }
        else { driving_mode = 0; }
    }
    // Obstacles can be destroyed north (6), south (8), east (7) and west (9)
    else if(movement == 6) {
        energy += ENERGY_DESTROY;
        printf("Trying to destroy obstacle (NORTH). ");
        if (world[robot_index - (width + 1)] == '*') {
            printf("SUCCESS.\n");
            world[robot_index - (width + 1)] = 'O';
        }
        else {
            printf("Nothing to destroy. Energy wasted.\n");
        }
        target_index = robot_index;
    }
    else if(movement == 7) {
        energy += 70;
        printf("Trying to destroy obstacle (EAST). ");
        if (world[robot_index+1] == '*') {
            printf("SUCCESS.\n");
            world[robot_index+1] = 'O';
        }
        else {
            printf("Nothing to destroy. Energy wasted.\n");
        }
        target_index = robot_index;
    }
    else if(movement == 8) {
        energy += 70;
        printf("Trying to destroy obstacle (SOUTH). ");
        if (world[robot_index + (width + 1)] == '*') {
            printf("SUCCESS\n");
            world[robot_index + (width + 1)] = 'O';
        }
        else {
            printf("Nothing to destroy. Energy wasted.\n");
        }
        target_index = robot_index;
    }
    else if(movement == 9) {
        energy += 70;
        printf("Trying to destroy obstacle (WEST). ");
        if (world[robot_index-1] == '*') {
            printf("SUCCESS\n");
            world[robot_index-1] = 'O';
        }
        else {
            printf("Nothing to destroy. Energy wasted.\n");
        }
        target_index = robot_index;
    }

    printf("Energy used so far: %d \n", energy);
    
    // ACTION
    if(world[target_index] == 'O') {
        if(driving_mode != 0) {
            print_world(world, width);
            printf("FAILURE, tried to drive on land in water mode!");
            return -2;
            //exit(1);
        }
        else {
            world[target_index] = 'R';
            world[robot_index] = coming_from;
            coming_from = 'O';
            return target_index;
        }
    }
    // Stay in the same location (used for driving mode toggling)
    else if(world[target_index] == 'R') {
        return target_index;
    }
    // Walls
    else if(world[target_index] == '#') {
        print_world(world, width);
        printf("FAILURE, crashed into a wall! \n");
        return -2;
        // exit(1);
    }
    // Obstacles (can be destroyed later)
    else if(world[target_index] == '*') {
        print_world(world, width);
        printf("FAILURE, crashed into an obstacle! \n");
        return -2;
        // exit(1);
    }
    else if(world[target_index] == '~') {
        if(driving_mode != 1) {
            printf("FAILURE, entered water in land mode! \n");
            return -2;
            // exit(1);
        }
        else {
            world[target_index] = 'R';
            world[robot_index] = coming_from;
            coming_from = '~';
            return target_index;
        }
        
    }
    // 'T' on land, 't' on water
    else if(world[target_index] == 'T' || world[target_index] == 't') {
        if(world[target_index] == 'T') {
            coming_from = 'O';
        } else {
            coming_from = '~';
        }
        world[target_index] = 'R';
        world[robot_index] = coming_from;
        rescued++;
        return target_index;
    }
    else if(world[target_index] == 'X') {
        world[target_index] = 'R';
        world[robot_index] = coming_from;
        if(rescued > 0) {
            print_world(world, width);
            printf("SUCCESS, target returned home! \n");
            return -1;
            // exit(0);
        } else {
            coming_from = 'X';
            return target_index;
        }
    } else {
        // Catchall fail
        return -3;
    }
}

int main() {
    const int MAX_STEPS = 500;
    int step = 1;
    
    int movement;
    int width = 20; // excluding newlines

    // The maps
    char world1[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','T','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','R','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world2[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','R','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','T','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world3[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','R','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','T','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world4[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','R','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','T','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world5[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','#','#','#','#','#','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','#','T','O','O','#','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','#','#','#','O','#','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','R','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world6[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','T','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','~','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','~','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','R','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world7[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','~','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','R','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','~','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','T','~','~','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','O','~','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world8[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','~','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','R','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','*','*','*','*','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','T','~','~','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','~','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','#','O','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world9[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','T','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','*','*','*','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','R','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world10[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','~','~','~','~','~','#','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','t','~','#','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','~','~','#','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','~','~','#','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','O','~','~','~','#','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','R','~','~','~','#','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world11[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','T','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','*','R','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world12[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','~','t','~','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','*','*','*','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','R','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world13[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','O','O','#','#','#','#','#','#','#','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','#','O','O','T','O','O','#','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','#','O','O','O','O','O','#','O','O','O','O','O','R','O','#','\n',
        '#','O','O','O','O','*','O','O','#','O','O','#','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','#','#','#','*','#','#','#','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    // Set the world to use
    char *maps[] = {world1, world2, world3, world4, world5, world6, world7, world8};
    unsigned long number_of_maps = sizeof(maps)/sizeof(*maps);
    char world[200];
    int energy_scores[number_of_maps];

    // TODO: Loop over all maps here
    for(int i = 0; i < number_of_maps; i++) {
        memcpy(world, maps[i], sizeof(world));
        // Initialize target and robot positions
        // Assumes only one target, one robot
        unsigned int elements = sizeof(world)/sizeof(world[0]);
        // Initialize the index of the robot and the target.

        // Assumes exactly one robot and one target
        int robot_index;
        int target_index;

        for(int i = 0; i < elements; i++) {
            if (world[i] == 'R') {
                robot_index = i; facing = world[i];
            }
            if (world[i] == 'T' || world[i] == 't') { target_index = i; }
        }

        // Print the initial world
        printf("Starting position: \n");
        print_world(world, width);
        // Debug output
        // printf("Robot index: %i / target index: %i %c", robot_index, target_index, '\n');

        while (step <= MAX_STEPS)
        {
            printf("After step number %i: \n", step);

            movement = move(world, i);
            robot_index = update_world(movement, world, robot_index, width);
            if (robot_index >= 0)
            {
                print_world(world, width);
                step = step + 1;
            }
            else if (robot_index == -1)
            {
                // Reset
                step = 1;
                energy_scores[i] = energy;
                reset_world();
                break; // SUCCESS
            }
            else if (robot_index == -2)
            {
                // Reset
                step = 1;
                energy_scores[i] = -1;
                reset_world();
                break; // FAILURE
            }
        }
        if (step >= MAX_STEPS)
        {
            energy_scores[i] = -1;
            printf("FAILURE, maximum number of steps exceeded.");
        }
    }
    
    // Print an overview
    printf("\n*** SCORE *** \n");
    int energy_sum = 0;
    int fail_counter = 0;
    int total_scores = sizeof(energy_scores)/sizeof(energy_scores[0]);
    for(int i = 0; i < total_scores; i++) {
        if(energy_scores[i] > 0) {
            printf("Map %d: %d \n", i+1, energy_scores[i]);
            energy_sum += energy_scores[i];
        }
        else {
            printf("Map %d: FAIL \n", i);
            fail_counter++;
        }
    }
    int finished_runs = total_scores-fail_counter;
    if(finished_runs > 0) {
        printf("Total: %d \n", energy_sum);
        printf("Average: %d ", energy_sum/(total_scores-fail_counter));
    } else {
        printf("Total: FAILED ALL \n");
        printf("Average: FAILED ALL ");
    }
    if(fail_counter > 0) { printf("/ %d failures \n", fail_counter); }
    else {printf("\n"); }
}