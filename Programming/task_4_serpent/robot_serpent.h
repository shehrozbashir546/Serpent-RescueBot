#ifndef ROBOT_H
#define ROBOT_H

#define up 1
#define right 2
#define down 3
#define left 4 
#define drivemode 5
#define up_destroy 6
#define right_destroy 7
#define down_destroy 8 
#define left_destroy 9
#define dont_destroy 10
#define destroy 11

static int NoVertical= 0, NoHorizontal = 0;
static int water = 0, land = 1;

static int forbiddenlist[200];
static int badcounter = 0;

static int blacklist_reset=0;
static int countdown=0;
static int countup=0;
static int countleft=0;
static int countright=0;
// Function declaration
void blacklist(int blacklisted);
int forbidden(int forbidden);
int move(char* world, int map_id);
int driveMode(char infront);
int destructable(char infront);
int up_destructable(char *world, char infront);
int right_destructable(char *world, char infront);
int down_destructable(char *world, char infront);
int left_destructable(char *world, char infront);
//int safe(char *world, int target_index, int width, int elements);
#endif // ROBOT_H