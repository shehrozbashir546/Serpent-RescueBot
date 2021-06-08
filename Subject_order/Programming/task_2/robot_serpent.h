#ifndef ROBOT_H
#define ROBOT_H



// Function declaration
int move(char *world);
int safe(char *world,int robot_index, int target_index, int width, int elements,int lines, int distance);
#endif // ROBOT_H