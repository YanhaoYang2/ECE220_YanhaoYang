;This program uses the concept of stack to calculate the postfix expression, and gives the result either in the hexadecimal form or "Invalid expression" if there is something wrong with the post fix expression.
;Since there are many different subroutines in which registers take different functions, the assignment of register will be given in front of each subroutine,
.ORIG x3000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R1-holder of ASCII code of space
PRINT	    
	GETC		; Get the inout from keyboard 
	OUT		;Write it to the console 
	LD R1,SPACE 	; Detect if the input is a space, if it is diretly go back to PRINT
	NOT R1,R1   	; Negate R1
	ADD R1,R1,#1;
	ADD R1,R1,R0	; IF ZERO, BACK TO PRINT
	BRz PRINT
	BRnzp EVALUATE	;Otherwise, go evaluate
	


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R3- value to print in hexadecimal
;R5- Store of thhe result
;R4- Holder of each hexidecimal print
;R1- Digit Counter
;R2- Bit Counter
;R0-Temporary Register
PRINT_HEX
	JSR POP
	ADD R5,R5,R0	;R5 STORE THE RESULT
	ADD R3,R5,#0
COUNTLOOP2
	ADD R6,R6,#1		; point R6 to the next number
	LD R1,DIGIT_COUNTER	; initialize digit counter to 4
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
	BRnz DONE		; FINISHED
	BRnzp DIGIT_COUNT_LOOP	; continue the digit count loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyb
;R1-temporary register

EVALUATE
    	LD R1, EQUALS	;Detect if "=" occurs
   	NOT R1,R1	;
   	ADD R1,R1,#1	;
   	ADD R1,R1,R0	;
   	BRz JUDGE	;
 	LD R1, ZERO	; Get the additive inverse of ASCII code of 0
  	NOT R1,R1	;
    	ADD R1,R1,#1	;
	ADD R1,R1,R0	; See if the input >= 0
	BRn NOTN        ; input is not a number, go to notnumber section
	LD R1, NINE	; see if input <=9
  	NOT R1,R1	;
    	ADD R1,R1,#1	; 
	ADD R1,R1,R0	; See if input is a number
	BRnz NUMBER     ; if input is a numbder, go to NUMBER page
	BRnzp NOTN	;Otherwies, input is not a number, go to notnumber section

NUMBER
   	LD R1, ZERO	; Get the additive inverse of ASCII code of 0
  	NOT R1,R1	;
    	ADD R1,R1,#1	;
	ADD R0,R1,R0	; Decrease the ASCII code of input by 30 to equal the input number with the value stored in the stack
	JSR PUSH	;PUSH the number into the Stack
	BRnzp PRINT	;Go back to print another input
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R3 the holder of the first operand, the second popped out
;R4 the holder of the second operand, the first popped out
;R0, the result of calculation
;R1, the holder of the ASCII code of input char
NOTN
    	AND R1,R1,#0	;CLEAR R1
	ADD R1,R1,R0	;HOLD ASCII CODE OF INPUT
	JSR POP
	ADD R5,R5,#0	;SEE IF UNDERFLOW ONCE HAPPENED 
	BRp INVALID	;IF UNDERFLOW ONCE HAPPENED, INVALID EXPRESSION
	AND R4,R4,#0	;  CLEAR R4
	ADD R4,R4,R0	; HOLD THE FIRST POPPED OUT
    	JSR POP
	ADD R5,R5,#0	;SEE IF UNDERFLOW ONCE HAPPENED 
	BRp INVALID	;IF UNDERFLOW ONCE HAPPENED, INVALID EXPRESSION
	AND R3,R3,#0	;  CLEAR R3
	ADD R3,R3,R0	; HOLD THE SECOND POPPED OUT
	ADD R0,R1,#0
	LD R1, EX	; Get the additive inverse of ASCII code of ^
  	NOT R1,R1	;
    	ADD R1,R1,#1	;
	ADD R1,R1,R0	; See if the input = ^
	BRz EXP		;  if  ASCII code = ^
	LD R1, PL	; Get the additive inverse of ASCII code of +
  	NOT R1,R1	;
    	ADD R1,R1,#1	;
	ADD R1,R1,R0	; See if the input = +
	BRz PLUS	;  if  ASCII code = +
	LD R1, MI	; Get the additive inverse of ASCII code of -
  	NOT R1,R1	;
    	ADD R1,R1,#1	;
	ADD R1,R1,R0	; See if the input = -
	BRz MIN		;  if  ASCII code = -
	LD R1, MU	; Get the additive inverse of ASCII code of *
  	NOT R1,R1	;
    	ADD R1,R1,#1	;
	ADD R1,R1,R0	; See if the input = *
	BRz MUL		;  if  ASCII code = *
	LD  R1,DI	; Get the additive inverse of ASCII code of /
  	NOT R1,R1	;
    	ADD R1,R1,#1	;
	ADD R1,R1,R0	; See if the input = /
	BRz DIV		;  if  ASCII code = /
	BRnzp INVALID	; If none of these operators is applied, invalid expression
	
	


INVALID 
	LEA R0, INV	; point R0 to the address of the string
	PUTS
	BRnzp DONE
 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
PLUS	
;your code goes here
	ADD R0,R3,R4	;R0 Stores the sum of R3 and R4
	JSR PUSH
	BRnzp PRINT

;input R3, R4
;out R0
MIN	
	NOT R4,R4
	ADD R4,R4,#1
    ADD R0,R3,R4
	JSR PUSH
	BRnzp PRINT
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MUL	
    	AND R0,R0,#0
	ADD R3,R3,#0		;Determine is the first operand is negative
	BRnz ONENEG		;See if there is one negative value in the multiplication
	BRnzp ONEPOS		;if it is not negative, it is positive
ONEPOS
	ADD R4,R4,#0
	BRzp MULTILOOP		;If both postive, go to normal MULTILOOP
	NOT R4,R4
	ADD R4,R4,#1
	BRnzp MULTILOOPNEG	;Go to Loop for multiplying a negative result
ONENEG
	NOT R3,R3
	ADD R3,R3,#1
	ADD R4,R4,#0
	BRzp MULTILOOPNEG	;Go to Loop for multiplying a negative result
	NOT R4,R4
	ADD R4,R4,#1
	BRnzp MULTILOOP		;If both negative, go to normal MULTILOOP, for negative times negative =positive 
MULTILOOP	
	ADD R0,R0,R4
	ADD R3,R3,#-1
	BRnz DONEMULTI
	BRnzp MULTILOOP
MULTILOOPNEG	
	ADD R0,R0,R4
	ADD R3,R3,#-1
	BRnz DONEMULTINEG
	BRnzp MULTILOOPNEG
DONEMULTINEG
	NOT R0,R0
	ADD R0,R0,#1
	JSR PUSH
	BRnzp PRINT
DONEMULTI
	JSR PUSH
	BRnzp PRINT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
;This section is the copy of the above multiplication section, with the difference that it is prerpared for calling from EXP(Exponent/Power) Section.
;Instead of 
MULEX	
    	AND R0,R0,#0
	ADD R3,R3,#0
	BRnz ONENEGEX
	BRnzp ONEPOSEX
ONEPOSEX
	ADD R4,R4,#0
	BRzp MULTILOOPEX
	NOT R4,R4
	ADD R4,R4,#1
	BRnzp MULTILOOPNEGEX
ONENEGEX
	NOT R3,R3
	ADD R3,R3,#1
	ADD R4,R4,#0
	BRzp MULTILOOPNEGEX
	NOT R4,R4
	ADD R4,R4,#1
	BRnzp MULTILOOPEX
MULTILOOPEX	
	ADD R0,R0,R4
	ADD R3,R3,#-1
	BRnz DONEMULTIEX
	BRnzp MULTILOOPEX
MULTILOOPNEGEX	
	ADD R0,R0,R4
	ADD R3,R3,#-1
	BRnz DONEMULTINEGEX
	BRnzp MULTILOOPNEGEX
DONEMULTINEGEX
	NOT R0,R0
	ADD R0,R0,#1
	RET
DONEMULTIEX
	RET
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
DIV	
    	AND R0,R0,#0
    	NOT R4,R4
    	ADD R4,R4,#1
DIVILOOP
    	ADD R3,R3,R4;
    	 BRn DIVIDONE
    	ADD R0,R0,#1
    	BRnzp DIVILOOP
DIVIDONE
    	JSR PUSH
    	BRnzp PRINT
	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
; R1-Power count
; R3-the container of the result from each multiplication
;R4-the holder of R3's original value
;out R0
EXP     
   	 AND R1,R1,#0
   	 ADD R1,R1,R4
   	 ADD R4,R3,#0
EXPLOOP	  
  	  ADD R1,R1,#-1		; The power count should decrease by 1 after every multiplication 
  	  BRnz DONEEXPO		;
	  JSR MULEX		;
  	  ADD R3,R0,#0;		;
  	  BRnzp EXPLOOP
    
DONEEXPO
    	AND R0,R0,#0
    	ADD R0,R0,R3
    	JSR PUSH
    	BRnzp PRINT
	
JUDGE	LD R1, STACK_TOP  		;TEST if there is exactly one result in the stack
	LD R2, STACK_START
	ADD R1,R1,#1
	NOT R1,R1			;NEGATE R1
	ADD R1,R1,#1
	ADD R1,R1,R2			; SEE IF THEY ARE EQUAL
	BRz PRINT_HEX			; if equal, go pop and print
	BRnzp INVALID			; if not , Invalid



;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACK_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;
EQUALS 		.FILL x003D	;
ZERO        	.FILL x0030 ;
NINE		.FILL x0039;
PL           	.FILL x002B ;
MI           	.FILL x002D ;
MU		.FILL x002A
DI		.FILL x002F
SPACE	    	.FILL x0020
EX		.FILL x005E
DIGIT_COUNTER	.FILL #4	; 4 digit
BIT_COUNTER	.FILL #4	; 4 bit
A_MINUS_TEN	.FILL x0037	; ASCii value of A-10
NEGATIVE_JUDGE	.FILL #-1	; Value used for judging whether a number is negative
AT		.FILL x0040	; ascii value of @
INV		.STRINGZ "Invalid Expression"
DONE    HALT
.END
