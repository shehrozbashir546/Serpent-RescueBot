#include <stdio.h>
#include <stdlib.h>
 
#define STARTNODE	1
#define ENDNODE		2
#define BARRIER		3
 
typedef struct AStarNode
{
	int s_x; // coordinates (required for final output path)
	int s_y;
	 int s_g; // The distance from the starting point to this point (f can be obtained from g and h, here f is omitted, f = g + h)
	 int s_h; // distance from this point to the end point predicted by the heuristic function
	 int s_style; // Node type: start point, end point, obstacle
	 struct AStarNode * s_parent; // parent node
	 int s_is_in_closetable; // whether it is in the close table
	 int s_is_in_opentable; // whether it is in the open table
}AStarNode, *pAStarNode;
 
 AStarNode map_maze [10] [10]; // node array
 pAStarNode open_table [100]; // open table
 pAStarNode close_table [100]; // close table
int	   open_node_count;		// Number of nodes in the open table
int	   close_node_count;		// Number of nodes in the close table
 pAStarNode path_stack [100]; // stack to save the path
 int top = -1; // top of stack
 
 
 // swap two elements
// 
void swap( int idx1, int idx2 )  
{  
	pAStarNode tmp = open_table[idx1];
	open_table[idx1] = open_table[idx2];
	open_table[idx2] = tmp;
}  
 
 // Heap adjustment
// 
void adjust_heap( int /*i*/nIndex )    
{ 	
	int curr = nIndex;
	 int child = curr * 2 + 1; // get the idx of the left child (the subscript starts from 0, all children are curr * 2 + 1)
	 int parent = (curr-1) / 2; // get parent idx
 
	if (nIndex < 0 || nIndex >= open_node_count)
	{
		return;
	}
	
	 // Adjust downwards (to compare left and right children with cuur parent)
	// 
	while ( child < open_node_count )
	{
		 // Small root heap is the parent value is less than the child value
		// 
		if ( child + 1 < open_node_count && open_table[child]->s_g + open_table[child]->s_h  > open_table[child+1]->s_g + open_table[child+1]->s_h )
		{
			++child;				// determine the size of the left and right children
		}
		
		if (open_table[curr]->s_g + open_table[curr]->s_h <= open_table[child]->s_g + open_table[child]->s_h)
		{
			break;
		}
		else
		{
			 swap (child, curr); // swap node
			 curr = child; // judge the current child node
			 child = curr * 2 + 1; // judge the left child again
		}
	}
	
	if (curr != nIndex)
	{
		return;
	}
 
	 // Adjust upwards (only need to compare cuur child and parent)
	// 
	while (curr != 0)
	{
		if (open_table[curr]->s_g + open_table[curr]->s_h >= open_table[parent]->s_g + open_table[parent]->s_h)
		{
			break;
		}
		else
		{
			swap( curr, parent );
			curr = parent;
			parent = (curr-1)/2;
		}
	}
}  
 
 // Determine whether the neighbor can enter the open table
// 
void insert_to_opentable( int x, int y, pAStarNode curr_node, pAStarNode end_node, int w )
{
	int i;
 
	 if (map_maze [x] [y] .s_style!= BARRIER) // not an obstacle
	{
		 if (! map_maze [x] [y] .s_is_in_closetable) // Not in the closed table
		{
			 if (map_maze [x] [y] .s_is_in_opentable) // in the open table
			{
				 // Need to judge whether it is a more optimized path
				// 
				 if (map_maze [x] [y] .s_g> curr_node-> s_g + w) // if more optimized
				{
					map_maze[x][y].s_g = curr_node->s_g + w;
					map_maze[x][y].s_parent = curr_node;
 
					for ( i = 0; i < open_node_count; ++i )
					{
						if ( open_table[i]->s_x == map_maze[x][y].s_x && open_table[i]->s_y == map_maze[x][y].s_y )
						{
							break;
						}
					}
 
					 adjust_heap (i); // Adjustment point below
				}
			}
			 else // not in open
			{
				map_maze[x][y].s_g = curr_node->s_g + w;
				map_maze[x][y].s_h = abs(end_node->s_x - x ) + abs(end_node->s_y - y);
				map_maze[x][y].s_parent = curr_node;
				map_maze[x][y].s_is_in_opentable = 1;
				open_table[open_node_count++] = &(map_maze[x][y]);
			}
		}
	}
}
 
 // Find neighbors
 // Find 8 neighbors up, down, left and right
//  
void get_neighbors( pAStarNode curr_node, pAStarNode end_node )
{
	int x = curr_node->s_x;
	int y = curr_node->s_y;
 
	 // Let's deal with 8 neighbors below!
	// 
	if ( ( x + 1 ) >= 0 && ( x + 1 ) < 10 && y >= 0 && y < 10 )
	{
		insert_to_opentable( x+1, y, curr_node, end_node, 10 );
	}
 
	if ( ( x - 1 ) >= 0 && ( x - 1 ) < 10 && y >= 0 && y < 10 )
	{
		insert_to_opentable( x-1, y, curr_node, end_node, 10 );
	}
 
	if ( x >= 0 && x < 10 && ( y + 1 ) >= 0 && ( y + 1 ) < 10 )
	{
		insert_to_opentable( x, y+1, curr_node, end_node, 10 );
	}
 
	if ( x >= 0 && x < 10 && ( y - 1 ) >= 0 && ( y - 1 ) < 10 )
	{
		insert_to_opentable( x, y-1, curr_node, end_node, 10 );
	}
 
	if ( ( x + 1 ) >= 0 && ( x + 1 ) < 10 && ( y + 1 ) >= 0 && ( y + 1 ) < 10 )
	{
		insert_to_opentable( x+1, y+1, curr_node, end_node, 14 );
	}
 
	if ( ( x + 1 ) >= 0 && ( x + 1 ) < 10 && ( y - 1 ) >= 0 && ( y - 1 ) < 10 )
	{
		insert_to_opentable( x+1, y-1, curr_node, end_node, 14 );
	}
 
	if ( ( x - 1 ) >= 0 && ( x - 1 ) < 10 && ( y + 1 ) >= 0 && ( y + 1 ) < 10 )
	{
		insert_to_opentable( x-1, y+1, curr_node, end_node, 14 );
	}
 
	if ( ( x - 1 ) >= 0 && ( x - 1 ) < 10 && ( y - 1 ) >= 0 && ( y - 1 ) < 10 )
	{
		insert_to_opentable( x-1, y-1, curr_node, end_node, 14 );
	}
}
 
 
int main()
{ 
	 // The definition of the map array
	// 
	 AStarNode * start_node; // start point
	 AStarNode * end_node; // End point
	 AStarNode * curr_node; // current point
	 int is_found; // Whether to find the path
	 int maze [] [10] = {// just assign to map_maze for good
						{ 1,0,0,3,0,3,0,0,0,0 },
						{ 0,0,3,0,0,3,0,3,0,3 },
						{ 3,0,0,0,0,3,3,3,0,3 },
						{ 3,0,3,0,0,0,0,0,0,3 },
						{ 3,0,0,0,0,3,0,0,0,3 },
						{ 3,0,0,3,0,0,0,3,0,3 },
						{ 3,0,0,0,0,3,3,0,0,0 },
						{ 0,0,2,0,0,0,0,0,0,0 },
						{ 3,3,3,0,0,3,0,3,0,3 },
						{ 3,0,0,0,0,3,3,3,0,3 },
					};
	int		  i,j,x;
	
	 // Prepare below
	// 
	for( i = 0; i < 10; ++i )
	{
		for ( j = 0; j < 10; ++j )
		{
			map_maze[i][j].s_g = 0;
			map_maze[i][j].s_h = 0;
			map_maze[i][j].s_is_in_closetable = 0;
			map_maze[i][j].s_is_in_opentable = 0;
			map_maze[i][j].s_style = maze[i][j];
			map_maze[i][j].s_x = i;
			map_maze[i][j].s_y = j;
			map_maze[i][j].s_parent = NULL;
 
			 if (map_maze [i] [j] .s_style == STARTNODE) 
			{
				start_node = &(map_maze[i][j]);
			}
			 else if (map_maze [i] [j] .s_style == ENDNODE) 
			{
				end_node = &(map_maze[i][j]);
			}
 
			printf("%d ", maze[i][j]);
		}
 
		printf("\n");
	}
 
	 // The following uses the A * algorithm to get the path
	// 	
	 open_table [open_node_count ++] = start_node; // add the start point to the open table
	
	 start_node-> s_is_in_opentable = 1; // join the open table
	start_node->s_g = 0;
	start_node->s_h = abs(end_node->s_x - start_node->s_x) + abs(end_node->s_y - start_node->s_y);
	start_node->s_parent = NULL;
	
	if ( start_node->s_x == end_node->s_x && start_node->s_y == end_node->s_y )
	{
		 printf ("Start point == End point! \n");
		return 0;
	}
	
	is_found = 0;
 
	while( 1 )
	{
		// for test
		// 
/*		for ( x = 0; x < open_node_count; ++x )
		{
			printf("(%d,%d):%d   ", open_table[x]->s_x, open_table[x]->s_y, open_table[x]->s_g+open_table[x]->s_h);
		}
		printf("\n\n");
*/
		 curr_node = open_table [0]; // The first point of the open table must be the point with the smallest f value (obtained by heap sorting)
		 open_table [0] = open_table [-open_node_count]; // Put the last point on the first point, and then adjust the heap
		 adjust_heap (0); // adjust the heap
		
		 close_table [close_node_count ++] = curr_node; // add the current point to the close table
		 curr_node-> s_is_in_closetable = 1; // Already in the close table
 
		if (curr_node-> s_x == end_node-> s_x && curr_node-> s_y == end_node-> s_y) // The end point is in close, end
		{
			is_found = 1;
			break;
		}
 
		 get_neighbors (curr_node, end_node); // Deal with neighbors
 
		 if (open_node_count == 0) // no path arrives
		{
			is_found = 0;
			break;
		}
	}
 
	if ( is_found )
	{
		curr_node = end_node;
		
		while( curr_node )
		{
			path_stack[++top] = curr_node;
			curr_node = curr_node->s_parent;
		}
 
		 while (top >= 0) // Here is the output path ~
		{
			if ( top > 0 )
			{
				printf("(%d,%d)-->", path_stack[top]->s_x, path_stack[top--]->s_y);
			}
			else
			{
				printf("(%d,%d)", path_stack[top]->s_x, path_stack[top--]->s_y);
			}
		}
	}
	else
	{
		 printf ("Why did you find the path");
	}
 
	puts("");
 
	return 0;
}
 