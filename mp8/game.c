/* yanhaoy2
The functions in this file are used in main.c to play the popular game 2048.Implemented functions include  make_game, remake_game,
get_cell, move_w, move_s, move_a, move_d, and legal_move_check

The make game function takes the number of rows and columns and then creates a new game struct, with rows, cols, score, and 
a cells array initialized.

The remake game function takes a game struct and a new amount of rows and columns, and overwrites the old game to hold a new
game with the specified amount of rows and columns.

The get_cell function takes a row and column and returns a pointer to the given cell that is being referred to.

The move function are the meat of this assignment, and move the cells in the specified direction, combining them when they
move into a cell with the same value. All the move functions work the same way, which uses a triple nested for loop 
technique to move cells and then compare them to the cells above, below, or to the side of them, depending on which button
is pressed.

The legal_move_check determines if there are any moves left. It does that by checking if there are any empty cells, and then
checking if there are any neighbor cells with the same value, and returns 1 if either is true. 

In addition, I write an Merge function to help me with the merge action within each move.
*/




#include "game.h"


game * make_game(int rows, int cols)
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    mygame->rows = rows;
    mygame->cols = cols;
    mygame->score = 0;
   for(int i = 0; i < rows*cols; i++) {
     mygame->cells[i] = -1; //initialize the intial value of -1 to every empty cell
      }

   return mygame; // pointer returned
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/     
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	(*_cur_game_ptr)->rows = new_rows; //new row assigned
	(*_cur_game_ptr)->cols = new_cols;// new column assigned
	(*_cur_game_ptr)->score = 0;      // score cleared to 0
	for(int i = 0; i < new_rows*new_cols; i++) {
	  (* _cur_game_ptr)->cells[i] = -1; //reinitialize each cell to an empty cell by assigning the value -1
      }
    
	return;	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}
void MERGE(game* curgame, int dimension, int row,int column, int displacement) {
  /*this method merges the two cells into one cell on the game board, dimension means we are changing in row (0) or columns(1), 
row and column  means the row or cols(based on dimensions) of the current cell we want to merge to another location, 
the displacement tells us the direction of the merge of the distance between two cells,
 merges toward up(dimension = 0, displacement <0) or down(dimension = 0,displacement >0)
 left(dimension = 1, dispalcement > 0)or right(dimentsion = 1,displacement< 0)*/
  cell* Cell = get_cell(curgame, row, column);// find the pointer to the current cell
  if(dimension == 0){// row is changing
    cell* DestinationCell = get_cell(curgame,row+displacement,column);//get the Destination where we are merging our current cell into
    *DestinationCell = *Cell+*Cell;// merge happened, the value at destined place doubled;
    *Cell = -1;//original cell emptied
  } else {// column is changing
    cell* DestinationCell = get_cell(curgame,row,column+displacement);//get the Destination where we are merging our current cell into
    *DestinationCell = *Cell+*Cell;// merge happened, the value at destined place doubled;
    *Cell = -1;//orignal cell emptied
  }
  
}
cell * get_cell(game * cur_game, int row, int col)

{
  if (row >= 0 && row < cur_game->rows && col >= 0 && col < cur_game-> cols) {// boundary check, if valid return the cell pointer of the given place
    return cur_game->cells+cur_game->cols*row+col;
  }
  return NULL; //if bound check fails return NULL
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{ int evermoved = 0;// check if any cell change values, if changed, it becomes 1
  for(int j = 0;j< cur_game->cols;j++) {// since it is moving up, we analyze this problem by dividing it into many different columns to solve
    int lastcombinedrow = -2; // the integer suggested in merging have happedned in the column, if happened, it becomes 1
    for(int i = 1; i< cur_game->rows;i++)  {// we start this movement by comparing the current row and the row above it, with the column fixed, thus we start the loop from second row
      cell* currentcell = get_cell(cur_game,i,j);
      if(*currentcell == -1){// if it is an empty cell, do nothing
      }
      else{
      int disR = 1;// the distance between two cells in row;
            cell* emptycell = get_cell(cur_game,i-1,j); // check the if there is an empty cell located above the current cell in the gaming board;
	    if(*emptycell == -1) {// if its above cell is empty, continue to find the uppermost empty cell
	      
	    while (*emptycell == -1&& i-disR >= 0) {// find the uppermost empty cell above the current cell,within the boundary	     
	      emptycell = get_cell(cur_game,i-disR,j);//refresh the location of the emptycell
	      disR++;
	    }
	    disR--;//disR gives the location of the filled position
	    }

	    cell* NextCell = get_cell(cur_game,i-disR,j);
		if(*currentcell == *NextCell && i-disR != lastcombinedrow) {// the condition for merging satisfied
		  cur_game->score += 2*(*currentcell); // merge assured, score changed
		  lastcombinedrow = i-disR;
		  disR *= -1;
		  MERGE(cur_game,0,i,j,disR); // Merge, an method I wrote to implement the whole merging process
		   // Merge happened
		  evermoved = 1;
		} else {// if merging condition unsatisfied, we move it next to another cell.
		 if(disR == 1&&*NextCell!=-1) {
		  }
		  else{
		    if(*NextCell==-1){ *(get_cell(cur_game,i-disR,j)) = *currentcell;
		  *currentcell = -1;
		  evermoved = 1;
		    }else{
		  *(get_cell(cur_game,i-disR+1,j)) = *currentcell;
		  *currentcell = -1;
		  evermoved = 1;}
		
		  }
	      
	    }
    
      }
    }
  }
  if(evermoved == 0) {// if no cell changed, it is an invalid move
    return 0;
  }
    return 1;
};

int move_s(game * cur_game) //slide down
{ int evermoved = 0;// check if any cell change values, if changed, it becomes 1
  for(int j = 0;j< cur_game->cols;j++) {// since it is moving up, we analyze this problem by dividing it into many different columns to solve
    int lastcombinedrow = -2; // the integer suggested in merging have happedned in the column, if happened, it becomes 1
    for(int i = cur_game->rows-2; i>=0;i--)  {// we start this movement by comparing the current row and the row above it, with the column fixed, thus we start the loop from second row
      cell* currentcell = get_cell(cur_game,i,j);
      if(*currentcell == -1){// if it is an empty cell, do nothing
      }
      else{
      int disR = 1;// the distance between two cells in row;
            cell* emptycell = get_cell(cur_game,i+1,j); // check the if there is an empty cell located above the current cell in the gaming board;
	    if(*emptycell == -1) {// if its above cell is empty, continue to find the uppermost empty cell
	      
	    while (*emptycell == -1&& i+disR <cur_game->rows) {// find the uppermost empty cell above the current cell,within the boundary	     
	      emptycell = get_cell(cur_game,i+disR,j);//refresh the location of the emptycell
	      disR++;
	    }
	    disR--;//disR gives the location of the filled position
	    }

	    cell* NextCell = get_cell(cur_game,i+disR,j);
		if(*currentcell == *NextCell && i+disR != lastcombinedrow) {// the condition for merging satisfied
		  cur_game->score += 2*(*currentcell); // merge assured, score changed
		  lastcombinedrow = i+disR;
		  MERGE(cur_game,0,i,j,disR); // Merge, an method I wrote to implement the whole merging process
		   // Merge happened
		  evermoved = 1;
		} else {// if merging condition unsatisfied, we move it next to another cell.
		 if(disR == 1&&*NextCell!=-1) {
		  }
		  else{
		    if(*NextCell==-1){ *(get_cell(cur_game,i+disR,j)) = *currentcell;
		  *currentcell = -1;
		  evermoved = 1;
		    }else{
		  *(get_cell(cur_game,i+disR-1,j)) = *currentcell;
		  *currentcell = -1;
		  evermoved = 1;}
		
		  }
	      
	    }
    
      }
    }
  }
  if(evermoved == 0) {// if no cell changed, it is an invalid move;
    return 0;
  }
    return 1;
    
   
}

int move_a(game * cur_game) //slide left
{
   
int evermoved = 0;// check if any cell change values, if changed, it becomes 1
  for(int i = 0;i< cur_game->rows;i++) {// since it is moving up, we analyze this problem by dividing it into many different columns to solve
    int lastcombinedcol = -2; // the integer suggested in merging have happedned in the column, if happened, it becomes 1
    for(int j = 1; j< cur_game->cols;j++)  {// we start this movement by comparing the current row and the row above it, with the column fixed, thus we start the loop from second row
      cell* currentcell = get_cell(cur_game,i,j);
      if(*currentcell == -1){// if it is an empty cell, do nothing
      }
      else{
      int disC = 1;// the distance between two cells in col;
            cell* emptycell = get_cell(cur_game,i,j-1); // check the if there is an empty cell located above the current cell in the gaming board;
	    if(*emptycell == -1) {// if its above cell is empty, continue to find the uppermost empty cell
	      
	    while (*emptycell == -1&& j-disC >= 0) {// find the uppermost empty cell above the current cell,within the boundary	     
	      emptycell = get_cell(cur_game,i,j-disC);//refresh the location of the emptycell
	      disC++;
	    }
	    disC--;//disR gives the location of the filled position
	    }

	    cell* NextCell = get_cell(cur_game,i,j-disC);
		if(*currentcell == *NextCell && j-disC != lastcombinedcol) {// the condition for merging satisfied
		  cur_game->score += 2*(*currentcell); // merge assured, score changed
		  lastcombinedcol = j-disC;
		  disC *= -1;
		  MERGE(cur_game,1,i,j,disC); // Merge, an method I wrote to implement the whole merging process
		   // Merge happened
		  evermoved = 1;
		} else {// if merging condition unsatisfied, we move it next to another cell.
		  if(disC == 1&&*NextCell!=-1) {
		  }
		  else{
		    if(*NextCell==-1){ *(get_cell(cur_game,i,j-disC)) = *currentcell;
		  *currentcell = -1;
		  evermoved = 1;
		    }else{
		  *(get_cell(cur_game,i,j-disC+1)) = *currentcell;
		  *currentcell = -1;
		  evermoved = 1;}
		
		  }
		
	      
	    }
    
      }
    }
  }
  if(evermoved == 0) {// if no cell changed, it is an invalid move;
    return 0;
  }
    return 1;
};

int move_d(game * cur_game) { //slide to the right
int evermoved = 0;// check if any cell change values, if changed, it becomes 1
  for(int i = 0;i< cur_game->rows;i++) {// since it is moving up, we analyze this problem by dividing it into many different columns to solve
    int lastcombinedcol = -2; // the integer suggested in merging have happedned in the column, if happened, it becomes 1
    for(int j = cur_game->cols-2; j>=0;j--)  {// we start this movement by comparing the current row and the row above it, with the column fixed, thus we start the loop from second row
      cell* currentcell = get_cell(cur_game,i,j);
      if(*currentcell == -1){// if it is an empty cell, do nothing
      }
      else{
      int disC = 1;// the distance between two cells in col;
            cell* emptycell = get_cell(cur_game,i,j+1); // check the if there is an empty cell located above the current cell in the gaming board;
	    if(*emptycell == -1) {// if its above cell is empty, continue to find the uppermost empty cell
	      
	    while (*emptycell == -1&& j+disC<cur_game->cols) {// find the uppermost empty cell above the current cell,within the boundary	     
	      emptycell = get_cell(cur_game,i,j+disC);//refresh the location of the emptycell
	      disC++;
	    }
	    disC--;//disR gives the location of the filled position
	    }

	    cell* NextCell = get_cell(cur_game,i,j+disC);
		if(*currentcell == *NextCell && j+disC != lastcombinedcol) {// the condition for merging satisfied
		  cur_game->score += 2*(*currentcell); // merge assured, score changed
		  lastcombinedcol = j+disC;
		  MERGE(cur_game,1,i,j,disC); // Merge, an method I wrote to implement the whole merging process
		   // Merge happened
		  evermoved = 1;
		} else {// if merging condition unsatisfied, we move it next to another cell.
		  if(disC == 1&&*NextCell!=-1) {
		  }
		  else{
		    if(*NextCell==-1){ *(get_cell(cur_game,i,j+disC)) = *currentcell;
		  *currentcell = -1;
		  evermoved = 1;
		    }else{
		  *(get_cell(cur_game,i,j+disC-1)) = *currentcell;
		  *currentcell = -1;
		  evermoved = 1;}
		
		  }
	    }
    
      }
    }
  }
  if(evermoved == 0) {// if no cell changed, it is an invalid move;
    return 0;
  }
    return 1;
}


int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //YOUR CODE STARTS HERE
  for(int i = 0; i < cur_game->rows;i++) {
    for(int j = 0; j< cur_game->cols;j++) {
      cell* cell =get_cell(cur_game,i,j);
      if(*cell==-1) {// if there is one empty cell, legal move exists
	  return 1;}
    
      //next we check if two adjacent blocks have the same value, if it is, legal move exists
      if( i - 1>=0) {
	if (*(get_cell(cur_game,i,j))==*(get_cell(cur_game,i-1,j))) {
	  return 1;
	}
      }
      if( i + 1<cur_game->rows) {
	if (*(get_cell(cur_game,i,j))==*(get_cell(cur_game,i+1,j))) {
	  return 1;
	}
      }
      if( j - 1>=0) {
	if (*(get_cell(cur_game,i,j))==*(get_cell(cur_game,i,j-1))) {
	  return 1;
	}
      }
      if( j + 1<cur_game->cols) {
	if (*(get_cell(cur_game,i,j))==*(get_cell(cur_game,i,j+1))) {
	  return 1;
	}
      }
  }
    
  }
    return 0;
}


/*! Code below is provided and should not be changed*/
void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
