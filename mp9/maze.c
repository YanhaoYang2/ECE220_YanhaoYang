/*The following file reads a file to create a maze and then find a solution for the maze that starts from S point and end at E point using recursive search method */

#include <stdio.h>
#include <stdlib.h>
#include "maze.h"


/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{ int width, height;
    char cur_cell;

    FILE * file = fopen(fileName, "r");  // Open the file for reading

    fscanf(file, "%d", &width);  // Get the width and height of the maze 
    fscanf(file, "%d", &height);
    fscanf(file,"%c",&cur_cell);

    maze_t * maze = malloc(sizeof(maze_t));  //  allocate memory of size of maze_t dynamically in heap

    maze->width = width;  
    maze->height = height;
    maze->cells = (char**)malloc( height * sizeof(char*) );  // Allocate memory for cells
    for (int p = 0; p < height; p++) {  // Loop through rows of array to create memory for each of them
        maze->cells[p] = (char*)malloc( width * sizeof(char) );
    }

  
    for (int i = 0; i < height; i++) {  
        for (int j = 0; j < width+1; j++) {  
	  fscanf(file,"%c",&cur_cell); 
            if (cur_cell == '\n') continue;  // If it is newline, continue
            if (cur_cell == 'S') {  // Starting position finded
                maze->startRow = i;
                maze->startColumn = j;
            }
            if (cur_cell == 'E') {  //Ending position finded
                maze->endRow = i;
                maze->endColumn = j;
            }
            maze->cells[i][j] = cur_cell;  // Copy file char to cells array
        }
    }

    fclose(file);  // Close file and return maze
    return maze;
   
}
/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
  for(int i = 0; i < maze->height;i++) {// free the double pointers first
    free(maze->cells[i]);
  }
  free(maze->cells); //free pointers secondly
  free(maze);//free the whole struct in the end
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
  for(int i = 0; i < maze->height;i++) {
    for(int j = 0; j < maze->width;j++){
      printf("%c",maze->cells[i][j]); }
    printf("\n");//new line switched
  }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{   char cur_cell = maze->cells[row][col];
  //Base Cases
  if(col == maze->endColumn && row==maze->endRow) return 1;// maze solved
  if(col < 0 || col>=maze->width || row < 0 ||row >=maze->height) return 0;// maze out of edge
  if (cur_cell == '%' || cur_cell == '*' || cur_cell == '~') return 0;  
  if (cur_cell != 'S') maze->cells[row][col] = '*'; 

  // RECURSE CASES
  if ( solveMazeDFS(maze, col + 1, row) ) return 1;  // Go to right cell
  if ( solveMazeDFS(maze, col, row - 1) ) return 1;  // Go to bottom cell
  if ( solveMazeDFS(maze, col, row + 1) ) return 1;  // Go to top cell
  if ( solveMazeDFS(maze, col - 1, row) ) return 1;  // Go to left cell
  if (cur_cell != 'S') maze->cells[row][col] = '~';
  return 0;

}
