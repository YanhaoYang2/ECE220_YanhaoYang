/*	This program deals with three functions in this game, setting the seed number by the input digit, starting the game through creating four numbers ranged from 1 to 8 from the seed
 * and officially start the game by asking players to guess four numbers and provide feedback accordingly.		
 *
 * prog5.c - source file adapted from UIUC ECE198KL Spring 2013 Program 4
 *           student code -- GOLD VERSION by Steven S. Lumetta
 */


/*
 * The functions that you must write are defined in the header file.
 * Blank function prototypes with explanatory headers are provided
 * in this file to help you get started.
 */



#include <stdio.h>
#include <stdlib.h>

#include "prog5.h"


/*
 * You will need to keep track of the solution code using file scope
 * variables as well as the guess number.
 */

static int guess_number;
static int solution1;
static int solution2;
static int solution3;
static int solution4;


/*
 * set_seed -- This function sets the seed value for pseudorandom
 * number generation based on the number the user types.
 * The input entered by the user is already stored in the string seed_str by the code in main.c.
 * This function should use the function sscanf to find the integer seed value from the 
 * string seed_str, then initialize the random number generator by calling srand with the integer
 * seed value. To be valid, exactly one integer must be entered by the user, anything else is invalid. 
 * INPUTS: seed_str -- a string (array of characters) entered by the user, containing the random seed
 * OUTPUTS: none
 * RETURN VALUE: 0 if the given string is invalid (string contains anything
 *               other than a single integer), or 1 if string is valid (contains one integer)
 * SIDE EFFECTS: initializes pseudo-random number generation using the function srand. Prints "set_seed: invalid seed\n"
 *               if string is invalid. Prints nothing if it is valid.
 */
int set_seed (const char seed_str[])
{ int seed;
  char post[2];
  int readeditem;
  readeditem = sscanf (seed_str, "%d%1s", &seed, post);
  //check if input is exactly an integer
  // if not print invalid and return 0
  if (readeditem != 1) {
    printf("set_seed: invalid seed\n");
    return 0;
  } else {
    srand(seed);
    return 1;
  }
  
//    Example of how to use sscanf to read a single integer and check for anything other than the integer
//    "int seed" will contain the number typed by the user (if any) and the string "post" will contain anything after the integer
//    The user should enter only an integer, and nothing else, so we will check that only "seed" is read. 
//    We declare
//    int seed;
//    char post[2];
//    The sscanf statement below reads the integer into seed. 
//    sscanf (seed_str, "%d%1s", &seed, post)
//    If there is no integer, seed will not be set. If something else is read after the integer, it will go into "post".
//    The return value of sscanf indicates the number of items read succesfully.
//    When the user has typed in only an integer, only 1 item should be read sucessfully. 
//    Check that the return value is 1 to ensure the user enters only an integer. 
//    Feel free to uncomment these statements, modify them, or delete these comments as necessary. 
//    You may need to change the return statement below
   
    return 0;
}


/*
 * start_game -- This function is called by main.c after set_seed but before the user makes guesses.
 *               This function creates the four solution numbers using the approach
 *               described in the wiki specification (using rand())
 *               The four solution numbers should be stored in the static variables defined above. 
 *               The values at the pointers should also be set to the solution numbers.
 *               The guess_number should be initialized to 1 (to indicate the first guess)
 * INPUTS: none
 * OUTPUTS: *one -- the first solution value (between 1 and 8)
 *          *two -- the second solution value (between 1 and 8)
 *          *three -- the third solution value (between 1 and 8)
 *          *four -- the fourth solution value (between 1 and 8)
 * RETURN VALUE: none
 * SIDE EFFECTS: records the solution in the static solution variables for use by make_guess, set guess_number
 */
void
start_game (int* one, int* two, int* three, int* four)
{
  //*one between 0 and 7
  *one = rand()%8;
  //*one betweeen 1 and 8
  *one += 1;
  *two = rand()%8;
  *two += 1;
  *three = rand()%8;
  *three += 1;
  *four = rand()%8;
  *four += 1;
   solution1 = *one;
  solution2 = *two;
  solution3 = *three;
  solution4 = *four;
  guess_number = 0;
}

/*
 * make_guess -- This function is called by main.c after the user types in a guess.
 *               The guess is stored in the string guess_str. 
 *               The function must calculate the number of perfect and misplaced matches
 *               for a guess, given the solution recorded earlier by start_game
 *               The guess must be valid (contain only 4 integers, within the range 1-8). If it is valid
 *               the number of correct and incorrect matches should be printed, using the following format
 *               "With guess %d, you got %d perfect matches and %d misplaced matches.\n"
 *               If valid, the guess_number should be incremented.
 *               If invalid, the error message "make_guess: invalid guess\n" should be printed and 0 returned.
 *               For an invalid guess, the guess_number is not incremented.
 * INPUTS: guess_str -- a string consisting of the guess typed by the user
 * OUTPUTS: the following are only valid if the function returns 1 (A valid guess)
 *          *one -- the first guess value (between 1 and 8)
 *          *two -- the second guess value (between 1 and 8)
 *          *three -- the third guess value (between 1 and 8)
 *          *four -- the fourth color value (between 1 and 8)
 * RETURN VALUE: 1 if the guess string is valid (the guess contains exactly four
 *               numbers between 1 and 8), or 0 if it is invalid
 * SIDE EFFECTS: prints (using printf) the number of matches found and increments guess_number(valid guess) 
 *               or an error message (invalid guess)
 *               (NOTE: the output format MUST MATCH EXACTLY, check the wiki writeup)
 */
int
make_guess (const char guess_str[], int* one, int* two, 
	    int* three, int* four)
{
  int w,x,y,z;
  //perfect match counter
  int rightnum = 0;
  //mismatch counter
  int mismatch = 0;
  char post[2];
//  The statement, given char post[2]; and four integers w,x,y,z,
  int readn = sscanf (guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post);
  // Test if the given input is exactly 4 numbers
  if (readn != 4){
    printf("make_guess: invalid guess\n");
    return 0;
  }
  // Test if input numbers have correct range
  if(w > 8 || w < 1 || x > 8 || x < 1 || y > 8 || y < 1 || z > 8 || z < 1){
    printf("make_guess: invalid guess\n");
    return 0;
  }
  guess_number++;
  *one = w;
  *two = x;
  *three = y;
  *four = z;
    int s1m = 0;
    int s2m = 0;
    int s3m =0;
    int s4m = 0;
    int g1m = 0;
    int g2m = 0;
    int g3m = 0;
    int g4m = 0;
    //Starting Comparison, s1m stands for solution 1 unmatched, and g1m stands for guess 1 (w) unmatched 
  if(w == solution1){
    rightnum++;
    s1m = 1;
    g1m = 1;
  }
  if(x == solution2){
    rightnum++;
    s2m = 1;
    g2m = 1;
  }
  if(y == solution3){
    rightnum++;
    s3m  = 1;
    g3m = 1;
  }
  if(z == solution4){
    rightnum++;
    s4m =1;
    g4m =1;
  }
  if(g1m == 0) {
    if(s2m == 0){
      if(w == solution2){
	mismatch+=1;
	g1m = 1;
	s2m = 1;

  }
    } if  (s3m == 0 && g1m == 0){
      if(w == solution3 && g1m == 0){
	mismatch+=1;
	g1m = 1;
	s3m = 1;

  }
     } if (s4m == 0 && g1m == 0){
      if(w == solution4){
	mismatch+=1;
	g1m = 1;
	s4m = 1;
      }
  }
  }
  if(g2m == 0) {
    if(s1m == 0){
      if(x == solution1){
	mismatch+=1;
	s1m = 1;
	g2m = 1;

  }
    }if (s3m == 0 && g2m == 0){
      if(x == solution3){
	mismatch+=1;
	s3m = 1;
	g2m = 1;

  }
     }if(s4m == 0 && g2m == 0){
      if(x == solution4){
	mismatch+=1;
	s4m = 1;
	g2m = 1;
      }
  }
  }
 if(g3m == 0) {
    if(s1m == 0){
      if(y == solution1){
	mismatch+=1;
	s1m = 1;
	g3m = 1;
  }
    }   if(s2m == 0 && g3m == 0){
      if(y == solution2){
	mismatch+=1;
	g3m = 1;
	s2m = 1;

  }
     }  if(s4m == 0 && g3m == 0){
      if(y == solution4){
	mismatch+=1;
	s4m = 1;
	g3m = 1;
      }
  }
  }
  if(g4m == 0) {
    if(s1m == 0){
      if(z == solution1){
	mismatch+=1;
	s1m = 1;
	g4m = 1;
  }
    }  if(s2m == 0 && g4m == 0){
      if(z == solution2){
	mismatch+=1;
	g4m = 1;
	s2m = 1;
  }
     }if(s3m == 0 && g4m == 0){
      if(z == solution3){
	mismatch+=1;
	g4m = 1;
	s3m = 1;
      }
  }
  }
  printf( "With guess %d, you got %d perfect matches and %d misplaced matches.\n", guess_number,rightnum,mismatch);
    return 1;
}


