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
Initialize_loop	STR R1,R2,#0	; store R1's value into the address stored in R2
		ADD R2,R2,#1	; increment R2 as a address pointer
		ADD R3,R3,#-1	; decrement R3 as a counter
		BRzp Initialize_loop	; if R3 > 0, continue Initialize_loop
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
		LD R1,List_addr	; let R1 holds the address x5000  
next_label		LDR R5,R1,#0	; let R5 holds the value stored at R1
		BRz Print		; if R5 = 0, branch to Print
		ADD R2,R1,#0	; let R2 holds R1's value
label_loop	ADD R1,R1,#1	; increment the address pointer
		LDR R5,R1,#0	; let R5 holds the value stored at R1
		BRnp label_loop	; check if R5 = 0, if so, continue
		ADD R1,R1,#1	; if R5 = 0, increment R1 so it points to bit vector
		LDR R3,R1,#0	; let R3 holds the bit vector
		ADD R1,R1,#1	; increment R1 so it points to slot number
		LDR R4,R1,#0	; let R4 holds the slot number
		BRn error1	; if slot number < 0, branch to error1
		ADD R5,R4,#-15	; check if slot number > 15
		BRp error1	; if slot number > 15, branch to error1
 
		; translate the address of label to corresponding address
		ADD R5,R4,R4	; calculate the offset, which is 5*R4, let R5 holds it
		ADD R5,R5,R5	; calculate the offset, which is 5*R4, let R5 holds it
		ADD R5,R5,R4	; calculate the offset, which is 5*R4, let R5 holds it
		LD R6,PTR_ADDR	; let R6 holds the value x4000 firstly
		ADD R6,R6,R5	; let R6 holds the value: x4000 + offset
		AND R5,R3,#1	; check if bit[0] is 1
		BRz check_1	; if bit[0] is 0, then check bit[1]
		LDR R5,R6,#0	; let R5 store the value in target address
		BRnp error2	; if the value is not x0000, branch to error2
		STR R2,R6,#0	; store the value of R2 into the target address
check_1		AND R5,R3,#2	; check if bit[1] is 1
		BRz check_2	; if bit[1] is 0, then check bit[2]
		LDR R5,R6,#1	; let R5 store the value in target address
		BRnp error2	; if the value is not x0000, branch to error2
		STR R2,R6,#1	; store the value of R2 into the target address
check_2		AND R5,R3,#4	; check if bit[2] is 1
		BRz check_3	; if bit[2] is 0, then check bit[3]
		LDR R5,R6,#2	; let R5 store the value in target address
		BRnp error2	; if the value is not x0000, branch to error2
		STR R2,R6,#2	; store the value of R2 into the target address
check_3		AND R5,R3,#8	; check if bit[3] is 1
		BRz check_4	; if bit[3] is 0, then check bit[4]
		LDR R5,R6,#3	; let R5 store the value in target address
		BRnp error2	; if the value is not x0000, branch to error2
		STR R2,R6,#3	; store the value of R2 into the target address
check_4		AND R5,R3,#16	; check if bit[4] is 1
		BRz ready_for_next	; if bit[4] is 0, then branch to ready_for_next
		LDR R5,R6,#4	; let R5 store the value in target address
		BRnp error2	; if the value is not x0000, branch to error2
		STR R2,R6,#4	; store the value of R2 into the target address
ready_for_next	ADD R1,R1,#1	; increment R1 so it points to the next_label
		BRnzp next_label	; branch to next_label
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
		ADD R1,R1,R4	; let R1 add the offset to point to next string
First_loop		LD R0,Vertical_line	; let R0 stores x7C to print a vertical line
		OUT		; print a vertical line
		JSR PRINT_CENTERED	; call PRINT_CENTERED to print the string
		ADD R1,R1,R4	; let R1 add the offset to point to next string
		ADD R3,R3,#-1	; decrement R3 as a days counter
		BRp First_loop	; if R3 > 0, branch to First_loop and countine
		LD R0,Line_feed	; if R3 = 0, let R0 stores x0A to print the line feed
		OUT		; print the line feed
		
		; Print the subsequent line
		LD R5,PTR_ADDR	; initialize R5 with value x4000 to be a address pointer
		LD R2,Hour_counter	; let R2 stores value 0 as a hour counter
Hours_loop	ADD R1,R2,#0	; let R1 store the value of R2
		JSR PRINT_SLOT	; call PRINT_SLOT to print the hour
		LD R3,Days_counter	; let R3 stores value 5 as a days counter
Days_loop	LD R0,Vertical_line	; let R0 stores x7C to print a vertical line
		OUT		; print a vertical line
		LDR R1,R5,#0	; let R1 stores the address of the string
		BRnp Act		; if the address is not x0000, print the string
		LEA R1,empty	; if the address is x0000, let R1 stores the address of empty
Act		JSR PRINT_CENTERED	; call PRINT_CENTERED to print the string
		ADD R5,R5,#1	; increment R5 as a address pointer
		ADD R3,R3,#-1	; decrement R3 as a days counter
		BRp Days_loop	; if R3 > 0, branch to Days_loop and countine
		LD R0,Line_feed	; if R3 = 0, let R0 stores x0A to print the line feed
		OUT		; print the line feed
		ADD R2,R2,#1	; increment R2 as a hour counter
		ADD R6,R2,#-15	; check if R2 > 15
		BRnz Hours_loop	; if R2 <= 15, branch to Hours_loop and countine
		BRnzp DONE	; branch to DONE and finish the program
; Printing ends


Error1		ADD R0,R2,#0	; let R0 stores the address of the error label
		PUTS		; print the error label
		LEA R0,Error_message1	; let R0 holds the first address of the string
		PUTS		; print the string
		BRnzp DONE	; branch to DONE and finish the program
Error2		ADD R0,R2,#0	; let R0 stores the address of the error label
		PUTS		; print the error label
		LEA R0,Error_message2	; let R0 holds the first address of the string
		PUTS		; print the string
		BRnzp DONE	; branch to DONE and finish the program

DONE		HALT		; finish the program	

; initizlization data		
PTR_ADDR	.FILL	x4000
CNTR_1		.FILL	x004F 

; Translation data
List_addr		.FILL	x5000
Error_message1	.STRINGZ " has an invalid slot number.\n"
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

; what this subroutine PRINT_SLOT does:
; This subroutine can print one of the schedule slot hours. The printed value is 
; relevant to the value stored in R1, which equals to (R1 + 7):00 , and R1+7 is a 
; two-digit decimal number.

; how this subroutine be called:
; Using JSR to change the value of PC to the value of the address which the label 
; PRINT_SLOT points to can call this subroutine.

; how registers are used within PRINT_SLOT:
; R0 is used to store the address of the first character in a string that to be printed, 
; and R0's initial value is stored at address with label "R0_value".
; R1 holds a value from 0 to 15 that is relevant to the schedule slot hour to print.
; R2 is used to compute the offset of the address that to be printed, and R2's initial
; value is stored at address with label "R2_value".
; R7 holds the value of the address of the instruction after the subroutine call instruction
; and R7's initial value is stored at address with label "R7_value".
		
PRINT_SLOT	ST R0,R0_value1	; store the value of R0 at the address with label R0_value
		ST R2,R2_value1	; store the value of R2 at the address with label R2_value
		ST R7,R7_value1	; store the value of R7 at the address with label R7_value
		AND R2,R2,#0	; initialize the value of R2 with x0000
		LEA R0,char_7_1  	; let R0 stores the address of the char_7_1, which will be added the offset then
		ADD R2,R1,R1	; use two lines to let R2 stores the value of offset, which equals to 3 * R1
		ADD R2,R2,R1	; use two lines to let R2 stores the value of offset, which equals to 3 * R1
		ADD R0,R0,R2	; let Ro stores the address of the first character in a string that to be printed
		PUTS		; print the first two decimal figures, the address of the first character is stored in R0
		LEA R0,char_rest	; let R0 stores the address of the the colon, which followed by two zeroes and a trailing space
		PUTS		; print the colon, two zeroes and a trailing space
		LD R0,R0_value1	; let R0 store the initial value of R0
		LD R2,R2_value1	; let R2 store the initial value of R2
		LD R7,R7_value1	; lat R7 stores the value of the address of the instruction after the subroutine call instruction.
		JMP R7		; change the PC to the address of the instruction after the subroutine call instruction

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

; what this subroutine PRINT_CENTERED does:
; THis subroutine can print exactly six characters of a string, whose first ASCII
; character's address is stored in R1. If the string is longer than six characters, the
; subroutine will print the first six characters. If the string is shorter than six characters,
; the subroutine will print additional spaces around the string to bring the total length 
; to six characters. If the number of spaces needed is odd, the subroutine will use one 
; more trailing space than leading space.

; how this subroutine be called:
; Using JSR to change the value of PC to the value of the address which the label 
; PRINT_CENTERED points to can call this subroutine.

; how registers are used within PRINT_CENTERED:
; R0 is used to store the address of the first character in a string that to be printed, 
; and R0's initial value is stored at address with label "R0_value".
; R1 holds the address of the first ASCII character in sequence terminated by NUL, x00
; R2 is used to compute the length of the sequence and then used to compute the offset 
; of the address that to be printed, and R2's initial value is stored at address with label "R2_value".
; R3 is used as a counter when printing the six characters, and R3's initial value is stored at
; address with label "R3_value".
; R4 is used as temporary calculation, and R4's initial value is stored at address with label "R4_value".
; R7 holds the value of the address of the instruction after the subroutine call instruction.
; and R7's initial value is stored at address with label "R7_value".

PRINT_CENTERED	ST R0,R0_value2	; store the value of R0 at the address with label R0_value
		ST R1,R1_value2	; store the value of R1 at the address with label R1_value
		ST R2,R2_value2	; store the value of R2 at the address with label R2_value
		ST R3,R3_value2	; store the value of R3 at the address with label R3_value
		ST R4,R4_value2	; store the value of R4 at the address with label R4_value
		ST R7,R7_value2	; store the value of R7 at the address with label R7_value
		AND R2,R2,#0	; initialize the value of R2 with x0000
		AND R3,R3,#0	; initialize the value of R3 with x0000
		AND R4,R4,#0	; initialize the value of R4 with x0000
		ADD R4,R4,R1	; let R4 store the value of R1, which is the address of the first address
Compute_length	LDR R0,R4,#0	; let R0 store the ASCII codes of the first character
		ADD R2,R2,#1	; let R2 add 1 if there is a character is not x00, and the extra 1 will be subtracted later
		ADD R4,R4,#1	; let R4 add 1 so that the R4 will hold the next address
		ADD R0,R0,#0	; let branch check R0's value, which is the ASCII's value
		BRnp Compute_length	; if ASCII is not NUL, the computation continues and branch to Compute_length 
		ADD R2,R2,#-1	; if ASCII is NUL, the computation ends and subtract R2 the extra 1
		ADD R4,R2,#-6	; use R4 to check if R2, which is the length, more than 6
		BRnz Next_part	; if R4 <= #6, branch to Next_part
		AND R2,R2,#0	; if R4 > #6, initialize the value of R2 with x0000
		ADD R2,R2,#6	; then add the value of R2 to #6
Next_part		ADD R3,R2,#0	; R3 is used as a counter when printing the character, R3's value equals to R2
		ADD R4,R2,R2	; use R4 to calculate the offset, which equals to #4 * R2's value
		ADD R2,R4,R4	; use R4 to calculate the offset, which equals to #4 * R2's value
		LEA R0,Leading_space	; let R0 stores the address of first pattern of leading_space 
		ADD R0,R0,R2	; let R0 add the offset in R2 to the address of corresponding Leading_space pattern
		PUTS		; print the corresponding Leading_space pattern
Print_char		LDR R0,R1,#0	; let R0 stores the value of the address of the first character in the sequence to be printed 
		OUT		; print the first character
		ADD R1,R1,#1	; add #1 to R1 so that it points to the next character in the sequence
		ADD R3,R3,#-1	; substract #1 to R3 as a counter when printing a character
		BRp Print_char	; if R3 > 0, there is still some characters to be printed, branch to Print_char
		LEA R0,Trailing_space	; if R3 <= 0, all characters are printed, so let R0 stores the address of first pattern of Trailing_space 
		ADD R0,R0,R2	; let R0 add the offset in R2 to the address of corresponding Trailing_space pattern
		PUTS		; print the corresponding Trailing_space pattern
		LD R0,R0_value2	; let R0 store the initial value of R0
		LD R1,R1_value2	; let R1 store the initial value of R1
		LD R2,R2_value2	; let R2 store the initial value of R2
		LD R3,R3_value2	; let R3 store the initial value of R3
		LD R4,R4_value2	; let R4 store the initial value of R4
		LD R7,R7_value2	; lat R7 stores the value of the address of the instruction after the subroutine call instruction.
		JMP R7		; change the PC to the address of the instruction after the subroutine call instruction

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