/* This file implements three functions that maintains and updates the status of the game of Life,
 * This includes 
 * 1. countLiveNeighbor, which counts the number of alive neighbors of a given position
 * 2. updateBoard, which update the live/dead status of each element in this Game Board according to giv * en rules
 * 3. AliveStable, which checks if the alive cells are exactly the same in the next step
 *
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
  int livecounter = 0;
  /*Set the  Counter of the live neighbor*/
  for(int r = row - 1; r <= row + 1; r++) {
    if( r >=0 && r < boardRowSize){// Boundary check
       for (int c = col - 1; c <= col+1;c++){
         if(c >=0 && c < boardColSize) {//Boundary check
	   if(!(c == col && r== row)) {// if not the given point, test if it is alive
	     if(*(board+r*boardColSize+c)==1) {
	       livecounter++;
	     }
	   }
	 }
        }
       }
      
}
  /*Use the above for loop with boundary check that prevents out of boundary to calculate the number of living neighbors */
  return livecounter;
}

/*
 * Update the game board to the next step.
 * Input: 
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
  int copyboard[boardRowSize*boardColSize];// create an copyboard that stores the next generation of GameBoard, so that the updated value wouldn't disrupt other updates
  for (int r = 0; r < boardRowSize;r++ ) {
    for (int c = 0; c < boardColSize;c++){
      copyboard[r*boardColSize+c] = *(board+(r*boardColSize)+c);
    }
   }
  for (int r = 0; r < boardRowSize;r++ ) {
    for (int c = 0; c < boardColSize;c++){
      int count = countLiveNeighbor(board,boardRowSize,boardColSize,r,c);
	/*we use the fucntion to calculate the number of living neighbors*/
      if(*(board+(r*boardColSize)+c)== 1) {// judge the cell is alive or not 
	if(count == 2 || count == 3) {
	  *(copyboard+(r*boardColSize)+c)= 1;
	 
	}
	else {
	  *(copyboard+(r*boardColSize)+c)= 0;  /* if ther cell is alive and its alive neighbors numbers is not 2 or 3, it dies*/
	}
      }
      else {
	if (count == 3) {
	  *(copyboard+(r*boardColSize)+c)= 1;
	  /* if the cell is dead and its alive neigbors has exactly three, it turns alive*/
	}
	else { *(copyboard+(r*boardColSize)+c)= 0;
	}
      }
    }
  }
   for (int r = 0; r < boardRowSize;r++ ) {
    for (int c = 0; c < boardColSize;c++){
      board[r*boardColSize+c] = *(copyboard+(r*boardColSize)+c);//use the copyboard to update the newest generation to board
    }
   }
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with 
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */ 
int aliveStable(int* board, int boardRowSize, int boardColSize){
  int array[boardRowSize*boardColSize];
    /*Create an array to store the board data for later update*/
   for (int r = 0; r < boardRowSize;r++ ) {
    for (int c = 0; c < boardColSize;c++){
      array[r*boardColSize+c] = *(board+(r*boardColSize)+c);
    }
   }
   /* Copy the value of board into array*/
   int* arraypointer;
   arraypointer = array;
   updateBoard(arraypointer,boardRowSize,boardColSize);
   for (int r = 0; r < boardRowSize;r++ ) {
    for (int c = 0; c < boardColSize;c++){
      if(array[r*boardColSize+c]!= *(board+(r*boardColSize)+c)) {
	return 0;
      }
    }
   }
   /*Compare the updated array with original array,if anything change happens,return 0*/
  return 1;
}

				
				
			

