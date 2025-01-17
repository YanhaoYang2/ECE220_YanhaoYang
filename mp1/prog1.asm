;Partner: yeningl2
; The code given to you here implements the histogram calculation that 
; we developed in class.  In programming lab, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of 
; code to print the histogram to the monitor.
;
; If you finish your program, 
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string 
; terminated by a NUL character.  Lower case and upper case should 
; be counted together, and a count also kept of all non-alphabetic 
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance) 
; should be stored starting at x3F00, with the non-alphabetic count 
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram
	
	; fill the histogram with zeroes 
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z         ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop



PRINT_HIST

; you will need to insert your code to print the histogram here

; do not forget to write a brief description of the approach/algorithm
; for your implementation, list registers used in this part of the code,
; and provide sufficient comments
;
;
;
;
; The program prints out the number of occurence of 26 letters as well as 
; the total number of occurence of other characters inside a string with 
; the help of the histogram. The histogram already counts the number of 
; occurence, so the program below aims to print out those numbers from the histogram.
;
; table of register use in this part of the code
; 	R6 holds a pointer to the histogram (x3F00)
;	R1 holds the digit counter
;	R2 holds the bit counter
;	R3 holds the current number in histogram that need to be displayed
;	R4 holds the digit that is going to be displayed
;	R5 is the loop count
;	R0 is a temporary register

	LD R6,HIST_ADDR 	; point R6 to the start of the histogram
	LD R5,NUM_BINS		; Initialize loop count to 27

; the count loop for displaying histogram
COUNTLOOP2
	LDR R3,R6,#0 		; let R3 store the number to be displayed
	ADD R6,R6,#1		; point R6 to the next number
	LD R1,DIGIT_COUNTER	; initialize digit counter to 4

	LD R0,AT		; print the letter whose number is going to be displayed
	ADD R0,R0,#8	
	ADD R0,R0,#8
	ADD R0,R0,#8
	ADD R0,R0,#3
	NOT R5,R5
	ADD R5,R5,#1
	ADD R0,R5,R0
	ADD R5,R5,#-1
	NOT R5,R5
	TRAP x21		
	LD R0,SPACE
	TRAP x21		; add a space


DIGIT_COUNT_LOOP
	LD R2,BIT_COUNTER	; Initialize bit counter to 4
	AND R4,R4,#0		; Initialize R4 to 0

BIT_COUNT_LOOP
	ADD R4,R4,R4		; left shift R4
	LD R0,NEGATIVE_JUDGE	; set R0 to a negative number
	AND R0,R0,R3		; judge whether R3 is negative or positive
	BRzp ADD_ZERO_TO_DIGIT	; skip the instruction of add 1 to digit
	ADD R4,R4,#1		; add one to digit

ADD_ZERO_TO_DIGIT
	ADD R3,R3,R3		; left shift R3
	ADD R2,R2,#-1		; decrease bit counter
	BRp BIT_COUNT_LOOP	; countinue the bit count loop when bit counter>0
	
	ADD R0,R4,#-7		; Judge whether R4 is greater than 9
	ADD R0,R0,#-2
	BRnz ADD_ASCii_ZERO	; get the ascii value between 0 and 9
	BRnzp ADD_ASCii_A	; get the ascii value between A and F

ADD_ASCii_ZERO
	LD R0,ZERO		; R0 <-- ascii value of 0
	ADD R0,R4,R0		; get the ascii value of number 0-9
	BRnzp DISPLAY		; display the result

ADD_ASCii_A
	LD R0,A_MINUS_TEN	; R0 <-- ascii value of A-10
	ADD R0,R4,R0		; get the ascii value of A-F
	BRnzp DISPLAY		; display the result

DISPLAY
	TRAP x21		; output the value onto the monitor

	ADD R1,R1,#-1		; decrease digit count
	BRnz JUDGE_END		; go judge whether all the numbers are displayed
	BRnzp DIGIT_COUNT_LOOP	; continue the digit count loop


JUDGE_END
	ADD R5,R5,#-1		; decrease loop count
	BRnz DONE		; end the program when all 27 numbers are displayed
	LD R0,NEW_LINE		; change to the new line
	TRAP x21
	BRnzp COUNTLOOP2	; go to display the next letter's number

DONE	HALT			; done


; the data needed by the program
NUM_BINS	.FILL #27	; 27 loop iterations
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00     ; histogram starting address
STR_START	.FILL x4000	; string starting address

DIGIT_COUNTER	.FILL #4	; 4 digit
BIT_COUNTER	.FILL #4	; 4 bit
ZERO		.FILL x0030	; ASCii value of 0
A_MINUS_TEN	.FILL x0037	; ASCii value of A-10
NEGATIVE_JUDGE	.FILL #-1	; Value used for judging whether a number is negative
AT		.FILL x0040	; ascii value of @
NEW_LINE 	.FILL x000A	; ascii value of new line
SPACE 		.FILL x0020	; ascii value of space

; for testing, you can use the lines below to include the string in this
; program...
; STR_START	.FILL STRING	; string starting address
; STRING		.STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

	.END
