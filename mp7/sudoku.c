#include "sudoku.h"
//-------------------------------------------------------------------------------------------------
// This file implements five fucntions that are used to fill an incomplete sudoku board, using backtracking recursive principles
/* The first three functions determine whether the indicated value is in the given row, or given column, or the 3x3 zone the given address belongs to
/*The function is_val_valid combines the first three functions to determine if a value could be inserted in an unfilled spot in sudoku board
/* The final function solve_sudoku, uses is_val_valid and recursion to fill the unfilled sudoku board.
*/

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  int c;
  for (c = 0;c < 9; c++){ // check every element in the given row, if value matches, return true
    if(sudoku[i][c]== val){
      return 1;
    }
  }
  return 0;
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
   int r;
   for (r = 0;r < 9; r++){ // check every element in the given column, if value matches, return true;
    if(sudoku[r][j]== val){
      return 1;
    }
  }
  return 0;
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
   
  assert(i>=0 && i<9);
  
  // BEG TODO
  int r = i / 3 ;       // analyze which 3x3 zone this (i,j) location belongs to by dividng their value by 3
    int c = j / 3;
    for (int row = r*3; row < r*3+3; row ++ ) {// start from the first row of this zone to the end
      for (int col = c*3; col< c*3+3;col++){// start from the first column of this zone to the end
	if(sudoku[row][col]== val) { // if the value matches one value in this zone, return true;
	  return 1;
      }
    }
    }
  return 0;
  // END TODO

}
// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);

  // BEG TODO
  if(is_val_in_row(val, i, sudoku)==0 && is_val_in_col(val,j,sudoku)==0 && is_val_in_3x3_zone(val,i,j,sudoku) == 0){//if all three checks return 0, the value is ok in be filled in
    return 1;}
  return 0;
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEG TODO.
    int i, j, row, col, num;
  int flag = 1;
  for(i = 0; i < 9 && flag; i++){
    for(j = 0; j < 9 && flag; j++){ //check for first empty cell
      if(sudoku[i][j]==0){
        flag = 0;                   //set unfilled row and col to i and j, and set flag to 0
        row = i;
        col = j;
      }
    }
  }
  if(flag == 1){    //if board is filled, return 1
    return 1;
  }
  for(num = 1; num <= 9; num++){      //loop through possible values to fill cells
    if(is_val_valid(num,row,col,sudoku)==1){
      sudoku[row][col] = num;         //if num is valid, set cell
      if(solve_sudoku(sudoku)==1){    //then recurse until it returns 0 or 1
        return 1;                     //return 1 if done
      }
      sudoku[row][col] = 0;           //if does not recurse sucessfully, clear cell
    }                                 //and increment num
  }
  return 0;
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}





