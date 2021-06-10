#ifndef ROBOT_H
#define ROBOT_H

#define up 1
#define right 2
#define down 3
#define left 4 
#define CHANGE 1
#define drivemode 5


static int moveLeft = 0, moveRight = 0;
static int moveUp = 0, moveDown = 0;

static int NoVertical= 0, noHorizontal = 0;

static int onWater = 0, onLand = 1;//the robot start on land

static int avoid[200];
static int counter = 0;

// Function declaration
void Avoid(int avoidLocation);
int forbidden(int location);
int move(char* world);
int  ifToChange(char nextSurface);
//int safe(char *world, int target_index, int width, int elements);
#endif // ROBOT_H