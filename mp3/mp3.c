#include <stdio.h>
#include <stdlib.h>
/* This prgram prints a row from Pascal triangle. It asks user to input a row number 
and print out the coefficients.
 */
int main()
{
  int row;

  printf("Enter a row index: ");
  scanf("%d", &row);
 unsigned long int  output;
  for (int k = 0; k <= row; k++){
    output = 1;/*the output stores the value of each  coefficient */
    if(k >= 1){
   /* the following for loop calculates the coefficient that is printed each time */
    for (int i =  1; i <=k; i++){
      int mul = row;
      mul++;
      mul = mul -  i;
      output *= mul;
      output /= i;
    }
    }
    printf("%lu ", output); /*Print out the coefficient after each loop of calculation*/
  } 

  return 0;
}
