; This program can translate a student's daily schedule from a events list, which is stored in
; memory starting at address x5000, to a two-dimensional matrix pointers, which is stored in
; memory starting at address x4000, and then print the student's schedule with first line printing
; three -letter prefixes of days of the week and subsequent line beginning with an hour slot
; number. This program uses two subroutines PRINT_CENTERED and PRINT_SLOT.

		.ORIG x3000	; program begin at memory location x3000

; initialization begins
; registers use:
; R1 is used to store the value x0000 that to initialize memory locations x4000 through x404F
; R2 is used as a address pointer, pointing to the address to be initialized
; R3 is used as a counter, to check if the 80 locations are all initialized

		AND R1,R1,#0	; initialize R1 with value x0000
		LD R2,PTR_ADDR	; initialize R2 with value x4000 to be a address pointer
		LD R3,CNTR_1	; initialize R3 with value x004F to be a counter
Initialize_loop	STR R1,R2,#0	; initialize memory from x4000 to x404F with x0000
		ADD R2,R2,#1	
		ADD R3,R3,#-1	
		BRzp Initialize_loop	 
; initialization ends


; Translation begins
; registers use:
; R1 is used as  a address pointer starting at x5000
; R2 is used to holds the address of a label
; R3 is used to holds a bit vector of days of a event
; R4 is used to holds an hour slot number of a event
; R5 is used as temporary computation
; R6 is used to holds the target address

		; let R2,R3,R4 holds their value respectively
		LD R1,List_addr	; let R1 holds the address x5000 as pointer  
next_label	LDR R5,R1,#0	; if value in the address of R1 is x00, branch to Print
		BRz Print		
		ADD R2,R1,#0	; if not, let R2 stores the address
label_loop	ADD R1,R1,#1	; increase the pointer until the string ends
		LDR R5,R1,#0	
		BRnp label_loop	
		ADD R1,R1,#1	; when string ends, let pointer points to bit vector
		LDR R3,R1,#0	; let R3 holds the bit vector
		ADD R1,R1,#1	
		LDR R4,R1,#0	; let R4 holds the slot number
		BRn error1	; if slot number < 0 or > 15, print error1 message
		ADD R5,R4,#-15	
		BRp error1	
 
		; translate the address of label to corresponding address
		ADD R5,R4,R4	; calculate the offset, which is 5*R4, let R5 holds it
		ADD R5,R5,R5	
		ADD R5,R5,R4	
		LD R6,PTR_ADDR	; let R6 holds the value: x4000 + offset
		ADD R6,R6,R5	
		AND R5,R3,#1	; check if bit[0] in bit vector is 1
		BRz check_1	; if not, then check bit[1]
		LDR R5,R6,#0	; if yes, check if the value in target address is empty
		BRnp error2	; if not, print error2 message
		STR R2,R6,#0	; if yes, store the value of R2 into the target address
check_1		AND R5,R3,#2	; similar to the previous plate 
		BRz check_2	
		LDR R5,R6,#1	
		BRnp error2	
		STR R2,R6,#1	
check_2		AND R5,R3,#4	; similar to the previous plate
		BRz check_3	
		LDR R5,R6,#2	
		BRnp error2	
		STR R2,R6,#2	
check_3		AND R5,R3,#8	; similar to the previous plate
		BRz check_4	
		LDR R5,R6,#3	
		BRnp error2	
		STR R2,R6,#3	
check_4		AND R5,R3,#16	
		BRz ready_for_next	; similar to the previous plate
		LDR R5,R6,#4	
		BRnp error2	
		STR R2,R6,#4	
ready_for_next	ADD R1,R1,#1	; let pointer points to the next label
		BRnzp next_label	; then do the similar work to next label
; Translation ends


; Printing begins
; registers use:
; R0 is used to hold the ASCII value that to be printed next
; R1 is used to hold the hour slot number or the address of the string
; R2 is used as a counter for the hours in the outer loop
; R3 is used as a counter for the days in the inner loop
; R4 is used to hold the value of offset as #4
; R5 is used as a address pointer
; R6 is used as temporary computation

		; Print the first line
Print		LD R3,Days_counter	; let R3 stores value 5 as a days counter
		LD R4,offset	; let R4 stores value 4 as a offset
		LEA R1,First_line	; let R1 stores the address of the string		
		JSR PRINT_CENTERED	; call PRINT_CENTERED to print the string
		ADD R1,R1,R4	
First_loop		LD R0,Vertical_line	; print a vertical line
		OUT		
		JSR PRINT_CENTERED	; print the string
		ADD R1,R1,R4	
		ADD R3,R3,#-1	
		BRp First_loop	; if R3 > 0, branch to First_loop and countine
		LD R0,Line_feed	; if R3 = 0, print the line feed
		OUT		
		
		; Print the subsequent line
		LD R5,PTR_ADDR	; let R5 stores x4000 to be a address pointer
		LD R2,Hour_counter	; let R2 stores value 0 as a hour counter
Hours_loop	ADD R1,R2,#0	; let R1 stores the value of R2
		JSR PRINT_SLOT	; call PRINT_SLOT to print the hour
		LD R3,Days_counter	; let R3 stores value 5 as a days counter
Days_loop	LD R0,Vertical_line	; print a vertical line
		OUT		
		LDR R1,R5,#0	; let R1 stores the address of the string
		BRnp Act		; if the address is not x0000, print the string
		LEA R1,empty	; if the address is x0000, print empty string
Act		JSR PRINT_CENTERED	
		ADD R5,R5,#1	
		ADD R3,R3,#-1	
		BRp Days_loop	; if R3 > 0, branch to Days_loop and countine
		LD R0,Line_feed	; if not, print the line feed
		OUT		
		ADD R2,R2,#1	
		ADD R6,R2,#-15	; if R2 <= 15, branch to Hours_loop and countine
		BRnz Hours_loop	
		BRnzp DONE	; branch to DONE and finish the program
; Printing ends


Error1		ADD R0,R2,#0	; print the error label
		PUTS		
		LEA R0,Error_message1	; print the error1 string
		PUTS		
		BRnzp DONE	; branch to DONE and finish the program
Error2		ADD R0,R2,#0	; print the error label
		PUTS		
		LEA R0,Error_message2	; print the error2 string
		PUTS		
		BRnzp DONE	; branch to DONE and finish the program

DONE		HALT		; finish the program	

; initizlization data		
PTR_ADDR	.FILL	x4000
CNTR_1		.FILL	x004F 

; Translation data
List_addr		.FILL	x5000
Error_message1	.STRINGZ	" has an invalid slot number.\n"
Error_message2	.STRINGZ	" conflicts with an earlier event.\n"

;Printing data
Hour_counter	.FILL	x0000
Days_counter	.FILL	x0005
offset		.FILL	x0004
Vertical_line	.FILL	x007C
Line_feed		.FILL	x000A
First_line		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
		.STRINGZ	"Mon"
		.STRINGZ	"Tue"
		.STRINGZ	"Wed"
		.STRINGZ	"Thu"
		.STRINGZ	"Fri"
Empty		.FILL	x0000


; register use:
; R0 is used to store the address of the first character in a string that to be printed
; R1 holds a value from 0 to 15 that is relevant to the schedule slot hour to print.
; R2 is used to compute the offset of the address that to be printed
; R7 holds the value of the address of the instruction after the subroutine call instruction.
		
PRINT_SLOT	ST R0,R0_value1	; save the value of R0,R2,R7 respectively
		ST R2,R2_value1	
		ST R7,R7_value1	
		AND R2,R2,#0	; initialize the value of R2 with x0000
		LEA R0,char_7_1  	; let R0 stores the address and add 3 * R1 as offset to be a pointer
		ADD R2,R1,R1	
		ADD R2,R2,R1	
		ADD R0,R0,R2	; print the first two decimal figures
		PUTS		
		LEA R0,char_rest	; print the colon, two zeroes and a trailing space
		PUTS		
		LD R0,R0_value1	; load the value of R0,R2,R7 respectively
		LD R2,R2_value1	
		LD R7,R7_value1	
		RET

R0_value1	.FILL	x0000
R2_value1	.FILL	x0000
R7_value1	.FILL	x0000
char_7_1		.STRINGZ	"07"
		.STRINGZ	"08"
		.STRINGZ	"09"
		.STRINGZ	"10"
		.STRINGZ	"11"
		.STRINGZ	"12"
		.STRINGZ	"13"
		.STRINGZ	"14"
		.STRINGZ	"15"
		.STRINGZ	"16"
		.STRINGZ	"17"
		.STRINGZ	"18"
		.STRINGZ	"19"
		.STRINGZ	"20"
		.STRINGZ	"21"
		.STRINGZ	"22"
char_rest		.STRINGZ	":00 "

; register use:
; R0 is used to store the address of the first character in a string that to be printed.
; R1 holds the address of the first ASCII character in sequence terminated by NUL, x00.
; R2 is used to compute the length of the sequence and then used to compute the offset 
; of the address that to be printed.
; R3 is used as a counter when printing the six characters.
; R4 is used as temporary calculation and a pointer when compute string's length.
; R7 holds the value of the address of the instruction after the subroutine call instruction.

PRINT_CENTERED	ST R0,R0_value2	; save the value of R0,R1,R2,R3,R4,R7 respectively
		ST R1,R1_value2	
		ST R2,R2_value2	
		ST R3,R3_value2	
		ST R4,R4_value2	
		ST R7,R7_value2	
		AND R2,R2,#0	; initialize the value of R2,R3,R4 with x0000
		AND R3,R3,#0	
		AND R4,R4,#0	
		ADD R4,R4,R1	; let R4 store the value of R1 to be a pointer
Compute_length	LDR R0,R4,#0	; compute the length of the string
		ADD R2,R2,#1	; let R2 add 1 if there is a character is not x00, and the extra 1 will be subtracted later
		ADD R4,R4,#1	
		ADD R0,R0,#0	; if the char is not x00, continue 
		BRnp Compute_length
		ADD R2,R2,#-1	
		ADD R4,R2,#-6	; if length > 6, save the first six chars
		BRnz Next_part	; if not, branch to Next_part
		AND R2,R2,#0	
		ADD R2,R2,#6	
Next_part	ADD R3,R2,#0	; R3 is used as a counter when printing the character
		ADD R4,R2,R2	; let R4 holds offset, which is 4 * R2
		ADD R2,R4,R4	
		LEA R0,Leading_space	; print the corresponding Leading_space pattern
		ADD R0,R0,R2	
		PUTS		
Print_char	LDR R0,R1,#0	; if the char is not x00, print the char
		BRz Next_char
		OUT		
Next_char	ADD R1,R1,#1	
		ADD R3,R3,#-1	
		BRp Print_char	
		LEA R0,Trailing_space	; print the corresponding Trailing_space pattern
		ADD R0,R0,R2	
		PUTS		
		LD R0,R0_value2	; load the value of R0,R1,R2,R3,R4,R5,R6,R7 respectively
		LD R1,R1_value2	
		LD R2,R2_value2	
		LD R3,R3_value2	
		LD R4,R4_value2	
		LD R7,R7_value2	
		RET

R0_value2	.FILL	x0000
R1_value2	.FILL	x0000
R2_value2	.FILL	x0000
R3_value2	.FILL	x0000
R4_value2	.FILL	x0000
R7_value2	.FILL	x0000		
Leading_space	.STRINGZ	"   "
		.STRINGZ	"  "
		.FILL	x0000
		.STRINGZ	"  "
		.FILL	x0000
		.STRINGZ	" "
		.FILL	x0000
		.FILL	x0000
		.STRINGZ	" "
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
Trailing_space	.STRINGZ	"   "
		.STRINGZ	"   "
		.STRINGZ	"  "
		.FILL	x0000
		.STRINGZ	"  "
		.FILL	x0000
		.STRINGZ	" "
		.FILL	x0000
		.FILL	x0000
		.STRINGZ	" "
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000
		.FILL	x0000	

		.END