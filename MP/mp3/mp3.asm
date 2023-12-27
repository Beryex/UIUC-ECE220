; This program can translate a student's daily schedule from a events list, which is stored in
; memory starting at address x5000, to a two-dimensional matrix pointers, which is stored in
; memory starting at address x4000. Then the program can find a compatible combination of  
; times at which events that started at x6000 can be inserted into the existing weekly schedule
; or all possible combinations are eliminated as incompatible using a stack. The program finally
; print the student's schedule with first line printing three -letter prefixes of days of the week 
; and subsequent line beginning with an hour slot number.This program has a main part and 
; four subroutines: Initialization, Translation, Insertion and Print. And Print uses two subroutines 
; PRINT_CENTERED and PRINT_SLOT.

; Main_part begins
; Registers use:
; R1 is used to check if Translation works successfully
; R2 is used to check if Insertion works successfully

		.ORIG x3000	; program begin at memory location x3000

		AND R1,R1,#0	; initialize R1,R2 with value x0000 respectively
		AND R2,R2,#0
		JSR Initialization	; initialization memory location from x4000 to x404F
		JSR Translation	; Translate events at x5000 to schedule at x4000
		ADD R1,R1,#0	; check if Translation works successfully
		BRp Next_Insert
		BRnzp DONE
Next_Insert	JSR Insertion	; Insert events at x6000 to schedule at x4000
		ADD R2,R2,#0	; check if 	Insertion works successfully
		BRp Next_Print
		BRnzp DONE
Next_Print	JSR Print
DONE		HALT		; finish the program

; Main_part ends


	
; initialization begins
; registers use:
; R1 is used to store the value x0000 that to initialize memory locations x4000 through x404F
; R2 is used as a address pointer, pointing to the address to be initialized
; R3 is used as a counter, to check if the 80 locations are all initialized

Initialization	ST R0,R0_Initial	; save the value of R0,R1,R2,R3,R7 respectively
		ST R1,R1_Initial
		ST R2,R2_Initial
		ST R3,R3_Initial
		ST R7,R7_Initial
		AND R1,R1,#0	; initialize R1 with value x0000
		LD R2,PTR_ADDR	; initialize R2 with value x4000 to be a address pointer
		LD R3,CNTR_1	; initialize R3 with value x004F to be a counter
Initialize_loop	STR R1,R2,#0	; initialize memory from x4000 to x404F with x0000
		ADD R2,R2,#1	
		ADD R3,R3,#-1	
		BRzp Initialize_loop	
		LD R0,R0_Initial	; load the value of R0,R1,R2,R3,R7 respectively
		LD R1,R1_Initial
		LD R2,R2_Initial
		LD R3,R3_Initial
		LD R7,R7_Initial
		RET

; initizlization data		
PTR_ADDR	.FILL	x4000
CNTR_1		.FILL	x004F
R0_Initial		.FILL	x0000 
R1_Initial		.FILL	x0000
R2_Initial		.FILL	x0000
R3_Initial		.FILL	x0000
R7_Initial		.FILL	x0000
; initialization ends



; Translation begins
; registers use:
; R1 is used as  a address pointer starting at x5000
; R2 is used to holds the address of a label
; R3 is used to holds a bit vector of days of a event
; R4 is used to holds an hour slot number of a event
; R5 is used as temporary computation
; R6 is used to holds the target address

Translation	ST R0,R0_Trans	; save the value of R0,R1,R2,R3,R4,R5,R6,R7 respectively
		ST R1,R1_Trans
		ST R2,R2_Trans
		ST R3,R3_Trans
		ST R4,R4_Trans
		ST R5,R5_Trans
		ST R6,R6_Trans
		ST R7,R7_Trans

		; let R2,R3,R4 holds their value respectively
		LD R1,List_addr	; let R1 holds the address x5000  
next_label	LDR R5,R1,#0	; if value in the address of R1 is x00, translation is done
		BRz Trans_Done	
		ADD R2,R1,#0	; increase the pointer until the string ends
label_loop	ADD R1,R1,#1	
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
		AND R5,R3,#1	; check if bit[0] is 1
		BRz check_1	; if not, then check bit[1]
		LDR R5,R6,#0	; check if the value in target address is empty
		BRnp error2	; if not, print error2 message
		STR R2,R6,#0	; store the value of R2 into the target address
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
check_4		AND R5,R3,#16	; similar to the previous plate
		BRz ready_for_next	
		LDR R5,R6,#4	
		BRnp error2	
		STR R2,R6,#4	
ready_for_next	ADD R1,R1,#1	; let pointer points to the next label
		BRnzp next_label	

Error1		ADD R0,R2,#0	; print the error label
		PUTS		
		LEA R0,Error_message1	; print the error1 string
		PUTS		
		BRnzp Trans_fails	
Error2		ADD R0,R2,#0	; print the error label
		PUTS		
		LEA R0,Error_message2	; print the string
		PUTS		
		BRnzp Trans_fails		

Trans_Done	LD R0,R0_Trans	; load the value of R0,R1,R2,R3,R4,R5,R6,R7 respectively
		LD R1,R1_Trans
		LD R2,R2_Trans
		LD R3,R3_Trans
		LD R4,R4_Trans
		LD R5,R5_Trans
		LD R6,R6_Trans
		LD R7,R7_Trans
		ADD R1,R1,#1	; let R1 holds value 1 means Translation works successfully
		RET
Trans_fails	LD R0,R0_Trans	; load the value of R0,R1,R2,R3,R4,R5,R6,R7 respectively
		LD R1,R1_Trans
		LD R2,R2_Trans
		LD R3,R3_Trans
		LD R4,R4_Trans
		LD R5,R5_Trans
		LD R6,R6_Trans
		LD R7,R7_Trans
		RET

; Translation data
List_addr		.FILL	x5000
R0_Trans		.FILL	x0000
R1_Trans		.FILL	x0000
R2_Trans		.FILL	x0000
R3_Trans		.FILL	x0000
R4_Trans		.FILL	x0000
R5_Trans		.FILL	x0000
R6_Trans		.FILL	x0000
R7_Trans		.FILL	x0000
Error_message1	.STRINGZ " has an invalid slot number.\n"
Error_message2	.STRINGZ	" conflicts with an earlier event.\n"
; Translation ends


; Insertion begins
; This part uses a stack with base at x8000 to perform depth-first search.
; The stack has structure like	'hours of event1'	x7FFD
;			'days of event1'	x7FFE
;			'address of event1'	x7FFF
;			'empty base'	x8000
; This means each event takes three memory locations in the stack, which includes
; the address of a string describing the event, a bit vector of days for the event, a bit 
; vector of hour slots for the event, and their address is decreasing.
; registers use:
; R0 is used to holds a bit vector of days of a event
; R1 is used to holds a bit vector of hours of a event
; R2 is used to cumpute the offset for days bit vector for target address.
; R3 is used to cumpute the offset for hours bit vector for target address. 
; R4 is used as a event address pointer starting at x6000 and to check if bit in days vector is 1.
; R5 is used as a push counter and a target address pointer starting at x4000 
; and temporary computation and to check if insert.
; R6 is used as a stack pointer starting at x8000 and to check if bit in hours vector is 1.
; R7 is used as temporary computation

Insertion		ST R0,R0_Insert	; save the value of R0,R1,R2,R3,R4,R5,R6,R7 respectively
		ST R1,R1_Insert
		ST R2,R2_Insert
		ST R3,R3_Insert
		ST R4,R4_Insert
		ST R5,R5_Insert
		ST R6,R6_Insert
		ST R7,R7_Insert
		LD R4,event_addr	; initialize R4,R6 with value x6000,x8000 respectively
		LD R6,stack_addr
		AND R7,R7,#0	; initialize the base of stack with value x0000
		STR R7,R6,#0

		; PUSH one event at a time
PUSH		LD R5,R5_counter	; let R5 holds 3 to be a counter
PUSH_loop	LDR R7,R4,#0	; load address, days bit vector, hours bit vector to R7 first
		BRz Insert_success	; then store it to the stack
		ADD R6,R6,#-1	
		STR R7,R6,#0	
		ADD R4,R4,#1
		ADD R5,R5,#-1
		BRp PUSH_loop

		; check if the event compatible
Check_event	AND R2,R2,#0	; initialize R2,R3 with value x0000
		AND R3,R3,#0
		LDR R1,R6,#0	; let R1 stores the hours bit vector
		ADD R6,R6,#1
		LDR R0,R6,#0	; let R0 stores the days bit vector
		ADD R6,R6,#1	; then R6 points to the address of a string describing the event
		ST R6,R6_pointer	; save R6 and then use R6 as hours bit checker
		AND R6,R6,#0	
		ADD R6,R6,#1
		ST R4,R4_pointer	; save R4 and then use R4 as days bit checker
		AND R4,R4,#0	
		ADD R4,R4,#1
Check_hours_bit	AND R7,R1,R6	; if the bit in hours bit vector is 0, then check the next hour
		BRz Next_hour
Check_days_bit	LD R5,target_addr	; initialize R5 with value x4000
		AND R7,R0,R4	; if the bit in days bit vector is 0, then check the next day
		BRz Next_day
		ADD R5,R5,R2	; let R5 add the days and hours offset to be the target address
		ADD R5,R5,R3
		LDR R7,R5,#0	; if the value in target day and time is empty, check next day
		BRz Next_day
		ST R6,R6_checker	; save R6 and then use R6 as stack pointer
		LD R6,R6_pointer
		ST R5,R5_pointer	; save R5 and use R5 as temporary computation
		LDR R5,R6,#0	; check if the string in target day and time is in stack
		NOT R5,R5
		ADD R7,R7,R5
		LD R5,R5_pointer	; load R5 as the target address pointer
		ST R6,R6_pointer	; save R6 and then use R6 as a hours bit checker
		LD R6,R6_checker
		ADD R7,R7,#1
		BRnp Next_hour	; if not, check next hour
		AND R7,R7,#0	; if yes, delete it
		STR R7,R5,#0	
		AND R7,R7,#0	; let R5_checkr_2 holds 1 means hace deleted
		ADD R7,R7,#1
		ST R7,R5_checker_2
		BRnzp Next_day
Next_day		ADD R2,R2,#1	; increment the offset in R2
		ADD R4,R4,R4	; shift the days bit vector one place to the left
		ADD R7,R4,#-8	; if R4 > 16, all days have been checked
		ADD R7,R7,#-8
		BRp Check_insert	; chekc if insert
		BRnz Check_days_bit
Check_insert	LD R5,R5_checker_1
		BRz Insert_act	; when R5_checker_1 is 0, should insert
		LD R7,R5_checker_3
		AND R7,R5,R7	; when R5_checker_1 and R5_checker_3 are both 1 should insert
		BRp Insert_act
		LD R7,R5_checker_2
		BRz Next_hour	
		AND R7,R7,#0	; if have deleted, let R5_checker_3 holds 1
		ADD R7,R7,#1
		ST R7,R5_checker_3
		BRnzp Next_hour
Next_hour	AND R4,R4,#0	; initialize days bit checker with value x0001	
		ADD R4,R4,#1
		AND R2,R2,#0	; initialize days offset with value x0000
		ADD R3,R3,#5	; increment the offset in R3
		ADD R6,R6,R6	; shift the hours bit checker one place to the left
		BRnp Check_hours_bit
		BRz Need_pop
Insert_act		AND R7,R7,#0	; initialize all checkers with 0
		ST R7,R5_checker_1
		ST R7,R5_checker_2
		ST R7,R5_checker_3
		AND R4,R4,#0	; initialize days bit checker with value x0001	
		ADD R4,R4,#1
		AND R2,R2,#0	; initialize days offset with value x0000
Check_days_bit_act	LD R5,target_addr	; initialize R5 with value x4000
		AND R7,R0,R4	; if the bit in days bit vector is 0, then check the next day
		BRz Next_day_act
		ADD R5,R5,R2	; let R5 add the days and hours offset to be the target address
		ADD R5,R5,R3
		ST R6,R6_checker	; save R6 and then use R6 as the stack pointer
		LD R6,R6_pointer
		LDR R7,R6,#0	; insert the event
		STR R7,R5,#0
		ST R6,R6_pointer	; save R6 and then use R6 as a hours bit checker
		LD R6,R6_checker
		BRnzp Next_day_act
Next_day_act	ADD R2,R2,#1	; increment the offset in R2
		ADD R4,R4,R4	; shift the days bit vector one place to the left
		ADD R7,R4,#-8	; if R4 > 16, this event's insertion is done
		ADD R7,R7,#-8
		BRp Next_event	; after insertion, insert next event
		BRnz Check_days_bit_act
Next_event	LD R6,R6_pointer	; load R4,R6 to be pointer
		LD R4,R4_pointer
		ADD R6,R6,#-2	; prepare to push the next event
		BRnzp PUSH
Need_pop	LD R6,R6_pointer	; load R4,R6 to be pointer
		LD R4,R4_pointer
		AND R7,R7,#0	; initialize R5_checker_2 and R5_checker_3 with 0
		ST R7,R5_checker_2
		ST R7,R5_checker_3
		ADD R7,R7,#1	; let R5_checker_1 be 1 means need removed
		ST R7,R5_checker_1
		ADD R6,R6,#1	; change relative pointer
		ADD R4,R4,#-3
		LDR R7,R6,#0	; if the stack is empty, Insertion fails
		BRz Insert_fails
		BRnzp Check_event
Insert_success	LD R0,R0_Insert	; load the value of R0,R1,R2,R3,R4,R5,R6,R7 respectively
		LD R1,R1_Insert
		LD R2,R2_Insert
		LD R3,R3_Insert
		LD R4,R4_Insert
		LD R5,R5_Insert
		LD R6,R6_Insert
		LD R7,R7_Insert
		ADD R2,R2,#1	; let R2 holds value 1 means Insertion works successfully
		RET
Insert_fails	LEA R0,Error_message3	; print Error_message3
		PUTS
		LD R0,R0_Insert	; load the value of R0,R1,R2,R3,R4,R5,R6,R7 respectively
		LD R1,R1_Insert
		LD R2,R2_Insert
		LD R3,R3_Insert
		LD R4,R4_Insert
		LD R5,R5_Insert
		LD R6,R6_Insert
		LD R7,R7_Insert
		RET
; Insertion data
R0_Insert		.FILL	x0000
R1_Insert		.FILL	x0000
R2_Insert		.FILL	x0000
R3_Insert		.FILL	x0000
R4_Insert		.FILL	x0000
R5_Insert		.FILL	x0000
R6_Insert		.FILL	x0000
R7_Insert		.FILL	x0000
event_addr	.FILL	x6000
target_addr	.FILL	x4000
stack_addr	.FILL	x8000
R6_pointer	.FILL	x0000
R4_pointer	.FILL	x0000
R6_checker	.FILL	x0000
R4_checker	.FILL	x0000
R5_counter	.FILL	x0003
R5_pointer	.FILL	x0000
R5_checker_1	.FILL	x0000
R5_checker_2	.FILL	x0000
R5_checker_3	.FILL	x0000
Error_message3	.STRINGZ	"Could not fit all events into schedule.\n"
; Insertion ends


 
; Print begins
; registers use:
; R0 is used to hold the ASCII value that to be printed next
; R1 is used to hold the hour slot number or the address of the string
; R2 is used as a counter for the hours in the outer loop
; R3 is used as a counter for the days in the inner loop
; R4 is used to hold the value of offset as #4
; R5 is used as a address pointer
; R6 is used as temporary computation

Print		ST R0,R0_Print	; save the value of R0,R1,R2,R3,R4,R5,R6,R7 respectively
		ST R1,R1_Print
		ST R2,R2_Print
		ST R3,R3_Print
		ST R4,R4_Print
		ST R5,R5_Print
		ST R6,R6_Print
		ST R7,R7_Print
	
		; Print the first line
		LD R3,Days_counter	; let R3 stores value 5 as a days counter
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
		LD R5,PTR_ADDR_2		; let R5 stores x4000 to be a address pointer
		LD R2,Hour_counter	; let R2 stores value 0 as a hour counter
Hours_loop	ADD R1,R2,#0	; let R1 store the value of R2
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
		LD R0,R0_Print	; load the value of R0,R1,R2,R3,R4,R5,R6,R7 respectively
		LD R1,R1_Print
		LD R2,R2_Print
		LD R3,R3_Print
		LD R4,R4_Print
		LD R5,R5_Print
		LD R6,R6_Print
		LD R7,R7_Print
		RET

;Printing data
R0_Print		.FILL	x0000
R1_Print		.FILL	x0000
R2_Print		.FILL	x0000
R3_Print		.FILL	x0000
R4_Print		.FILL	x0000
R5_Print		.FILL	x0000
R6_Print		.FILL	x0000
R7_Print		.FILL	x0000
PTR_ADDR_2	.FILL	x4000
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
; Print ends


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
