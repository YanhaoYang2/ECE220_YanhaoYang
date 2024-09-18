// partners: yeningl2, yanhaoy2
#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>
//Introduction
//The program aims to use linked list to store a sparse matrix and do simple addition between two matrices.
//The program contains 8 functions, including 2 helper functions and 6 main functions
//The first helper function add_node adds a node to the linked list in the proper place
//The second helper function remove_node removes a node from a specific node from the linked list
//The first main function load_tuples loads a text file contains the basic information of a matrix into the program, and create a linked list that contains nonzero value of the matrix.
//The second main function gv_tuples returns the value of the matrix at a specific location.
//The third main function set_tuples changes the linked list by adding and removing elements according to the requirement
//The fourth main function save_tuples write the linked list onto a text file and store the matrix in it
//The fifth main function add_tuples creates a new linked list that stores the nonzero sum of two matrices
//The sixth main function destroy_tuples frees all the memory space allocated by the linked list


//a helper function to add a node to the linked list using recusion
void add_node(sp_tuples_node **node,int r, int c, double val){
  sp_tuples_node *head = *node; //a pointer that points to current node being examined
  sp_tuples_node *temp =  NULL; //a pointer created temporarily to add the node to the linked list
  if (head == NULL){ //base case 1, when the value to be inserted is at the last node
    (*node) = (sp_tuples_node *)malloc(sizeof(sp_tuples_node));
    (*node)->next = NULL;
    (*node)->row = r;
    (*node)->col = c;
    (*node)->value = val;
    return;
  }
  if (((head->row) == r && (head->col) > c) || ((head->row) > r)){//base case 2, insert the new node right in front of current head 
    temp = (sp_tuples_node*)malloc(sizeof(sp_tuples_node));
    temp->row = r;
    temp->col = c;
    temp->value = val;
    temp->next = NULL;
    *node = temp; //point the original head pointer to the new node created
    (*node)->next = head; // link current pointer to node right after the original head
    return;
  }
  if (head->row == r && head->col == c){ //base case 3, when the tuple already exists, but value needs to be modified
    (*node)->value = val;
    return;
  }
  add_node(&((*node)->next),r,c,val);
}

// a helper function to delete a node from the linked list using recursion
void remove_node(sp_tuples_node **node,int r, int c){
  sp_tuples_node *head = *node; //point to the node to be examined
  if (*node == NULL || (*node)->row > r){ //base case 1, when nothing needs to be removed
    return;
  }
  if ((*node)->row == r && (*node)->col == c){ //base case 2, when something needs to be removed
    *node = head->next;
    free(head);
    return;
  }
  remove_node(&((*node)->next),r,c);
}



sp_tuples * load_tuples(char* input_file)
{
  int row,col; //m stores the row of the matrix, n the col
  FILE *file;
  file = fopen(input_file,"r"); // read the file
  fscanf(file,"%d %d", &row, &col); //get the row and col of the matrix
  sp_tuples *tuple_start = (sp_tuples *)malloc(sizeof(sp_tuples)); //malloc sp_tuples that store the head pointer and row,col information of the matrix
  tuple_start->m = row; //initialize m
  tuple_start->n = col; //initialize n
  tuple_start->nz = 0; //initialize nz
  tuple_start->tuples_head = NULL;
  int get_row,get_col; //for each row of text file, the three values store the corresponding value
  double get_value;
  int flag = 1; //flag = -1 indicates the last line of the file is loaded onto the link list
  flag = fscanf(file,"%d %d %lf",&get_row,&get_col,&get_value); //get the row,col,value of a tuple,judge EOF
  while (flag != -1){
    set_tuples(tuple_start,get_row,get_col,get_value); //set the tuple with information given
    flag = fscanf(file,"%d %d %lf",&get_row,&get_col,&get_value); //get the row,col,value of a tuple,judge EOF
  }
  fclose(file); //close the file
  return tuple_start; //return the pointer
}


double gv_tuples(sp_tuples * mat_t,int row,int col)

{
  if (mat_t == NULL){
    return 0;
  }
  int curr_row,curr_col; //variables that store the current row and col of the node we are at
  sp_tuples_node *head = mat_t->tuples_head; //point the ptr head to the head pointer of the linked list
  while(head != NULL){ //when the corresponding row, col does not exist in the linked list, jump out of the loop
    curr_row = head->row;
    curr_col = head->col;
    if (curr_row == row && curr_col == col){ //when the corresponding node is found, get the value of the node
      return head->value;
    }
    else{
      head = head->next; //examine the next node when current node is not the target
    }
  }
  return 0;
}


void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
  // value to insert is zero
  if (value == 0){
    if (gv_tuples(mat_t,row,col)==0){//if the target row,col of matrix is already zero (not in the linked list)
      return; //end the function without doing anything
    }
    else{
      remove_node(&(mat_t->tuples_head),row,col); //remove the node
      (mat_t->nz)-=1;//update nonzero counter
      return;
    }
  }
  //value to insert is not zero
  if (gv_tuples(mat_t,row,col)==0){
    (mat_t->nz)+=1; //update nonzero counter
  }
  add_node(&(mat_t->tuples_head),row,col,value); //add the node
  return;
}


void save_tuples(char * file_name, sp_tuples * mat_t)
{
  if (mat_t == NULL){
    return;
  }
  FILE *out_file = fopen(file_name, "w"); //start to write the file
  sp_tuples_node *head = mat_t->tuples_head;
  
  int row,col; //row and col of the matrix
  row = mat_t->m; //get the number of rows
  col = mat_t->n; //get the number of cols
  fprintf(out_file,"%d %d\n",row,col); //first print out the # of rows and cols
  while (head != NULL){
    fprintf(out_file,"%d %d %lf\n",head->row,head->col,head->value);
    head = head->next;
  }
  fclose(out_file);
}



sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){
  sp_tuples_node *head_A = matA->tuples_head; //get the head of matrix A and B
  sp_tuples_node *head_B = matB->tuples_head;
  sp_tuples *matC = (sp_tuples *)malloc(sizeof(sp_tuples)); //create a marix C = A+B
  int row_A = matA->m; //get the #of row, col of A,B
  int row_B = matB->m;
  int col_A = matA->n;
  int col_B = matB->n;
  if ((row_A != row_B) || (col_A != col_B)){
    return NULL; // return NULL when the size of A,B doesn't match
  }
  matC->m = row_A; //initialize matrix C
  matC->n = col_A;
  matC->nz = 0;
  matC->tuples_head = NULL;
  while (head_A != NULL){
    int r,c; //row,col of current node
    double v_A,v_C; //value of current node
    r = (head_A)->row;
    c = (head_A)->col;
    v_C = gv_tuples(matC,r,c); //get C(r,c)
    v_A = gv_tuples(matA,r,c); //get A(r,c)
    set_tuples(matC,r,c,v_A+v_C); //C(i,j) = C(r,c)+A(r,c)
    head_A = (head_A)->next; //add the next node to C
  }
  while (head_B != NULL){ // do the same thing for matrix B
    int r,c; //row,col of current node
    double v_B,v_C; //value of current node
    r = head_B->row;
    c = head_B->col;
    v_C = gv_tuples(matC,r,c); //get C(r,c)
    v_B = gv_tuples(matB,r,c); //get B(r,c)
    set_tuples(matC,r,c,v_B+v_C); //C(r,c) = C(r,c)+B(r,c)
    head_B = head_B->next; //add the next node to C
  }
  return matC;
}


//sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){ 
//    return retmat;
    
//}


	
void destroy_tuples(sp_tuples * mat_t){
  if (mat_t == NULL){
    return;
  }
  sp_tuples_node *head = mat_t->tuples_head; //get the first node of matrix
  if (head == NULL){
    free(mat_t); //free the sp_tuples when tuples_head point to nothing;
  }
  sp_tuples_node *head_next = head->next; //head_next point to the node after what head points to
  while ((head_next) != NULL){
    free(head);
    head = head_next;
    head_next = head_next->next;
  }
  free(head);
  free(mat_t);
  return;
}
