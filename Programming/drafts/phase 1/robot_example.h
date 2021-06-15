#ifndef ROBOT_H
#define ROBOT_H

#define up 1
#define right 2
#define down 3
#define left 4 
#define drivemode 5
#define work 9
#define notwork 8

static int NoVertical= 0, noHorizontal = 0;

static int water = 0, land = 1;

static int forbiddenlist[200];
static int counter = 0;

// Function declaration
void blacklist(int blacklisted);
int forbidden(int forbidden);
int move(char* world);
int driveMode(char infront);
//int safe(char *world, int target_index, int width, int elements);
#endif // ROBOT_H