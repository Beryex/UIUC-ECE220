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

		.ORIG x3000	; begin at memory location x3000

; test codes begin
		LD	R0,BITS
		ADD	R2,R0,#0
		ADD	R3,R0,#0
		ADD	R4,R0,#0
		ADD	R5,R0,#0
		ADD	R6,R0,#0
		LEA	R1,test_str
		JSR	PRINT_CENTERED
		AND	R1,R1,#0
		ADD	R1,R1,#0
		JSR	PRINT_SLOT
		LD	R7,BITS
		NOT	R7,R7
		ADD	R7,R7,#1
		ADD	R0,R0,R7
		BRz	R0_OK
		LEA	R0,R0_BAD
		PUTS
R0_OK		ADD	R1,R1,R7
		BRz	R1_OK
		LEA	R0,R1_BAD
		PUTS
R1_OK		HALT

BITS	.FILL	xABCD	; something unusual
VLINE	.FILL	x7C	; ASCII vertical line character
R0_BAD	.STRINGZ "PRINT_SLOT changes R0!\n"
R1_BAD	.STRINGZ "PRINT_SLOT changes R1!\n"
test_str	.STRINGZ	"1012123"
 
	
; test codes end
		
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
		ST R2,R2_value2	; store the value of R2 at the address with label R2_value
		ST R3,R3_value2	; store the value of R3 at the address with label R3_value
		ST R4,R4_value2	; store the value of R4 at the address with label R4_value
		ST R7,R7_value2	; store the value of R7 at the address with label R7_value
		AND R2,R2,#0	; initialize the value of R2 with x0000
		AND R3,R3,#0	; initialize the value of R3 with x0000
		AND R4,R4,#0	; initialize the value of R4 with x0000
		ADD R4,R4,R1	; let R4 store the value of R1, which is the address of the first address
Compute_length	LDR R0,R4,#0	; let R0 store the ASCII codes f the first character
		ADD R2,R2,#1	; let R2 add 1 if there is a character is not x00, and the extra 1 will be subtracted later
		ADD R4,R4,#1	; let R4 add 1 so that the R4 will hold the next address
		ADD R0,R0,#0	; let branch check R0's value, which is the ASCII's value
		BRnp Compute_length	; if ASCII is not NUL, the computation continues and branch to Compute_length 
		ADD R2,R2,#-1	; if ASCII is NUL, the computation ends and subtract R2 the extra 1
		ADD R4,R2,#-6	; use R4 to check if R2, which is the length, more than 6
		BRnz Next_part	; if R4 <= #6, branch to Next_part
		AND R2,R2,#0	; if R4 > #6, initialize the value of R2 with x0000
		ADD R2,R2,#6	; then add the value of R2 to #6
Next_part		ADD R3,R2,R3	; R3 is used as a counter when printing the character, R3's value equals to R2
		ADD R4,R2,R2	; use R4 to calculate the offset, which equals to #4 * R2's value
		ADD R2,R4,R4	; use R4 to calculate the offset, which equals to #4 * R2's value
		LEA R0,Leading_space	; let R0 stores the address of first pattern of leading_space 
		ADD R0,R0,R2	; let R0 add the offset in R2 to the address of corresponding Leading_space pattern
		PUTS		; print the corresponding Leading_space pattern
Print_char		LDR R0,R1,#0	; let R0 stores the value of the address of the first character in the sequence to be printed
		BRz Next_char
		OUT		; print the first character
Next_char	ADD R1,R1,#1	; add #1 to R1 so that it points to the next character in the sequence
		ADD R3,R3,#-1	; substract #1 to R3 as a counter when printing a character
		BRp Print_char	; if R3 > 0, there is still some characters to be printed, branch to Print_char
		LEA R0,Trailing_space	; if R3 <= 0, all characters are printed, so let R0 stores the address of first pattern of Trailing_space 
		ADD R0,R0,R2	; let R0 add the offset in R2 to the address of corresponding Trailing_space pattern
		PUTS		; print the corresponding Trailing_space pattern
		LD R0,R0_value2	; let R0 store the initial value of R0
		LD R2,R2_value2	; let R2 store the initial value of R2
		LD R3,R3_value2	; let R3 store the initial value of R3
		LD R4,R4_value2	; let R4 store the initial value of R4
		LD R7,R7_value2	; lat R7 stores the value of the address of the instruction after the subroutine call instruction.
		JMP R7		; change the PC to the address of the instruction after the subroutine call instruction

R0_value2	.FILL	x0000
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

		.END		; END the assemble